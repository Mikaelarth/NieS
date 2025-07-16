#include "DatabaseManager.h"
#include <QSqlError>
#include <QProcessEnvironment>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent), m_settings("config.ini", QSettings::IniFormat)
{
    m_db = QSqlDatabase::addDatabase("QPSQL");
}

bool DatabaseManager::open()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    const QString host = env.contains("NIES_DB_HOST")
            ? env.value("NIES_DB_HOST")
            : m_settings.value("database/host", "localhost").toString();
    const QString dbName = env.contains("NIES_DB_NAME")
            ? env.value("NIES_DB_NAME")
            : m_settings.value("database/name").toString();
    const QString user = env.contains("NIES_DB_USER")
            ? env.value("NIES_DB_USER")
            : m_settings.value("database/user").toString();
    const QString password = env.contains("NIES_DB_PASSWORD")
            ? env.value("NIES_DB_PASSWORD")
            : m_settings.value("database/password").toString();
    const int port = env.contains("NIES_DB_PORT")
            ? env.value("NIES_DB_PORT").toInt()
            : m_settings.value("database/port", 5432).toInt();

    m_db.setHostName(host);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(user);
    m_db.setPassword(password);
    m_db.setPort(port);

    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        return false;
    }
    return true;
}

void DatabaseManager::close()
{
    if (m_db.isOpen())
        m_db.close();
}

QString DatabaseManager::lastError() const
{
    return m_lastError;
}
