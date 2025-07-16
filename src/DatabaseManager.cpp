#include "DatabaseManager.h"
#include <QSqlError>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent), m_settings("config.ini", QSettings::IniFormat)
{
    m_db = QSqlDatabase::addDatabase("QPSQL");
}

bool DatabaseManager::open()
{
    m_db.setHostName(m_settings.value("database/host", "localhost").toString());
    m_db.setDatabaseName(m_settings.value("database/name").toString());
    m_db.setUserName(m_settings.value("database/user").toString());
    m_db.setPassword(m_settings.value("database/password").toString());
    m_db.setPort(m_settings.value("database/port", 5432).toInt());

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
