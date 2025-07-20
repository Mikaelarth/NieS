#include <QtTest>
#include <QCommandLineParser>
#include <QTemporaryDir>
#include <QSettings>
#include "DatabaseManager.h"
#include "config_option_test.h"

void ConfigOptionTest::cliOverridesEnv()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    const QString envCfg = dir.filePath("env.ini");
    const QString cliCfg = dir.filePath("cli.ini");
    const QString localDb = dir.filePath("local.db");

    {
        QSettings s(envCfg, QSettings::IniFormat);
        s.setValue("database/offline", false);
    }
    {
        QSettings s(cliCfg, QSettings::IniFormat);
        s.setValue("database/offline", true);
        s.setValue("database/offline_path", localDb);
    }

    qputenv("NIES_CONFIG_PATH", envCfg.toUtf8());
    qunsetenv("NIES_DB_OFFLINE");
    qunsetenv("NIES_DB_OFFLINE_PATH");

    QCommandLineParser parser;
    QCommandLineOption configOpt("config", "Path to configuration file", "file");
    parser.addOption(configOpt);
    parser.process(QStringList{"testapp", "--config", cliCfg});

    DatabaseManager db(parser.value(configOpt));
    QVERIFY(db.open());
    QVERIFY(db.isOffline());
    db.close();

    qunsetenv("NIES_CONFIG_PATH");
}

#include "config_option_test.moc"
