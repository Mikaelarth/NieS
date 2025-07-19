#include <QCoreApplication>
#include <QSettings>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QSettings settings("config.ini", QSettings::IniFormat);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(settings.value("database/host", "localhost").toString());
    db.setDatabaseName(settings.value("database/name").toString());
    db.setUserName(settings.value("database/user").toString());
    db.setPassword(settings.value("database/password").toString());
    db.setPort(settings.value("database/port", 5432).toInt());

    if (!db.open()) {
        qCritical() << "Connection failed:" << db.lastError().text();
        return 1;
    }

    QSqlQuery query("SELECT version();");
    if (query.next()) {
        qInfo() << "PostgreSQL version:" << query.value(0).toString();
    } else {
        qCritical() << "Query error:" << query.lastError().text();
    }

    db.close();
    return 0;
}
