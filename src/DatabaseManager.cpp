#include "DatabaseManager.h"
#include <QtSql/QSqlError>
#include <QProcessEnvironment>
#include <QCoreApplication>

DatabaseManager::DatabaseManager(const QString &configPath, QObject *parent)
    : QObject(parent),
      m_settings([&]() {
          QString path = configPath;
          if (path.isEmpty()) {
              QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
              if (env.contains("NIES_CONFIG_PATH"))
                  path = env.value("NIES_CONFIG_PATH");
              else
                  path = QCoreApplication::applicationDirPath() + "/config.ini";
          }
          return path;
      }(), QSettings::IniFormat),
      m_db(QSqlDatabase::addDatabase("QMYSQL"))
{
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
            : m_settings.value("database/port", 3306).toInt();

    m_db.setHostName(host);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(user);
    m_db.setPassword(password);
    m_db.setPort(port);

    if (dbName.isEmpty() || user.isEmpty()) {
        m_lastError = QStringLiteral("Database settings are incomplete. "
                                      "Check config.ini or environment variables.");
        return false;
    }

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
