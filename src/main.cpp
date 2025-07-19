#include <QApplication>
#include <QMessageBox>
#include <QLocale>
#include <QTranslator>
#include "DatabaseManager.h"
#include "UserManager.h"
#include "UserSession.h"
#include "login/LoginDialog.h"
#include "login/MainWindow.h"
#include "NetworkMonitor.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    const QString locale = QLocale::system().name();
    const QString trPath = QCoreApplication::applicationDirPath() + "/translations";
    if (translator.load("NieS_" + locale, trPath)) {
        app.installTranslator(&translator);
    }

    DatabaseManager db;
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

    MainWindow mainWin(&session);
    mainWin.show();
    int ret = app.exec();

    db.close();
    return ret;
}
