#include <QApplication>
#include <QMessageBox>
#include <QLocale>
#include <QTranslator>
#include <QSettings>
#include <QProcessEnvironment>
#include <QCommandLineParser>
#include "DatabaseManager.h"
#include "UserManager.h"
#include "UserSession.h"
#include "login/LoginDialog.h"
#include "login/MainWindow.h"
#include "NetworkMonitor.h"
#include "dashboard/DashboardWindow.h"
#include "SalesManager.h"
#include "InventoryManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    QCommandLineOption configOption(QStringList{"config"},
                                   "Path to configuration file",
                                   "file");
    parser.addOption(configOption);
    parser.addHelpOption();
    parser.process(app);

    QString cmdConfigPath;
    if (parser.isSet(configOption))
        cmdConfigPath = parser.value(configOption);

    QTranslator translator;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    int dashInterval = 60000;
    {
        QString cfg = env.contains("NIES_CONFIG_PATH")
                ? env.value("NIES_CONFIG_PATH")
                : QCoreApplication::applicationDirPath() + "/config.ini";
        QSettings settings(cfg, QSettings::IniFormat);
        dashInterval = settings.value("dashboard/update_interval", dashInterval).toInt();
    }
    if (env.contains("NIES_DASH_INTERVAL"))
        dashInterval = env.value("NIES_DASH_INTERVAL").toInt();

    QString locale;
    if (env.contains("NIES_LANG"))
        locale = env.value("NIES_LANG");
    if (locale.isEmpty()) {
        QString cfg = env.contains("NIES_CONFIG_PATH")
                ? env.value("NIES_CONFIG_PATH")
                : QCoreApplication::applicationDirPath() + "/config.ini";
        QSettings settings(cfg, QSettings::IniFormat);
        locale = settings.value("app/language").toString();
    }
    if (locale.isEmpty())
        locale = QLocale::system().name();

    const QString trPath = QCoreApplication::applicationDirPath() + "/translations";
    if (translator.load("NieS_" + locale, trPath)) {
        app.installTranslator(&translator);
    }

    DatabaseManager db(cmdConfigPath);
    if (!db.open()) {
        QMessageBox::critical(nullptr, "Database Error", db.lastError());
        return -1;
    }

    NetworkMonitor monitor;
    QObject::connect(&monitor, &NetworkMonitor::connectivityChanged,
                     [&db](bool online) {
        if (online && db.isOffline())
            db.synchronize();
    });

    UserManager userManager;
    UserSession session(&userManager, &userManager);
    LoginDialog login(&session);
    if (login.exec() != QDialog::Accepted) {
        db.close();
        return 0;
    }

    SalesManager dashSm(&session);
    InventoryManager dashIm(&session);
    DashboardWindow dashboard(&dashSm, &dashIm, dashInterval);
    dashboard.show();

    MainWindow mainWin(&session);
    mainWin.show();
    int ret = app.exec();

    db.close();
    return ret;
}
