#include "DatabaseManager.h"
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QProcessEnvironment>
#include <QCoreApplication>
#include <QStringList>
#include <QDateTime>

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
      }(), QSettings::IniFormat)
{
}

bool DatabaseManager::open()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    m_paymentApiKey = env.contains("NIES_PAYMENT_API_KEY")
            ? env.value("NIES_PAYMENT_API_KEY")
            : m_settings.value("payment/api_key").toString();
    m_paymentEndpoint = env.contains("NIES_PAYMENT_ENDPOINT")
            ? env.value("NIES_PAYMENT_ENDPOINT")
            : m_settings.value("payment/endpoint").toString();

    m_offline = env.contains("NIES_DB_OFFLINE")
            ? !(env.value("NIES_DB_OFFLINE").isEmpty() || env.value("NIES_DB_OFFLINE") == "0" || env.value("NIES_DB_OFFLINE").toLower() == "false")
            : m_settings.value("database/offline", false).toBool();
    m_offlinePath = env.contains("NIES_DB_OFFLINE_PATH")
            ? env.value("NIES_DB_OFFLINE_PATH")
            : m_settings.value("database/offline_path", "nies_local.db").toString();
    m_driver = env.contains("NIES_DB_DRIVER")
            ? env.value("NIES_DB_DRIVER")
            : m_settings.value("database/driver", "QMYSQL").toString();

    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);

    if (m_offline) {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(m_offlinePath);

        if (!m_db.open()) {
            m_lastError = m_db.lastError().text();
            return false;
        }
        return true;
    }

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

    m_db = QSqlDatabase::addDatabase(m_driver);
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

bool DatabaseManager::synchronize()
{
    if (!m_offline)
        return true;

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

    QSqlDatabase remote = QSqlDatabase::addDatabase(m_driver, "sync");
    remote.setHostName(host);
    remote.setDatabaseName(dbName);
    remote.setUserName(user);
    remote.setPassword(password);
    remote.setPort(port);

    if (!remote.open()) {
        m_lastError = remote.lastError().text();
        QSqlDatabase::removeDatabase("sync");
        return false;
    }

    const QStringList tables = {"users", "products", "inventory", "sales"};
    for (const QString &table : tables) {
        QSqlQuery local(m_db);
        if (!local.exec(QString("SELECT * FROM %1").arg(table)))
            continue;
        QSqlRecord rec = local.record();
        while (local.next()) {
            QSqlQuery check(remote);
            check.prepare(QString("SELECT COUNT(*) FROM %1 WHERE id=?").arg(table));
            check.addBindValue(local.value("id"));
            if (!check.exec() || !check.next())
                continue;
            bool exists = check.value(0).toInt() > 0;

            // Determine timestamp column if any
            QString tsCol;
            if (rec.indexOf("updated_at") != -1)
                tsCol = "updated_at";
            else if (rec.indexOf("last_update") != -1)
                tsCol = "last_update";
            else if (rec.indexOf("created_at") != -1)
                tsCol = "created_at";

            if (exists) {
                if (!tsCol.isEmpty()) {
                    QSqlQuery remoteRow(remote);
                    remoteRow.prepare(QString("SELECT * FROM %1 WHERE id=?").arg(table));
                    remoteRow.addBindValue(local.value("id"));
                    if (remoteRow.exec() && remoteRow.next()) {
                        QDateTime localTs = QDateTime::fromString(local.value(tsCol).toString(), Qt::ISODate);
                        if (!localTs.isValid())
                            localTs = QDateTime::fromString(local.value(tsCol).toString(), "yyyy-MM-dd HH:mm:ss");
                        QDateTime remoteTs = QDateTime::fromString(remoteRow.value(tsCol).toString(), Qt::ISODate);
                        if (!remoteTs.isValid())
                            remoteTs = QDateTime::fromString(remoteRow.value(tsCol).toString(), "yyyy-MM-dd HH:mm:ss");
                        if (localTs <= remoteTs)
                            continue; // remote has newer or equal data
                    } else {
                        continue;
                    }
                } else {
                    continue; // record exists and no timestamp to compare
                }

                // Update remote with local newer row
                QStringList sets;
                for (int i = 0; i < rec.count(); ++i) {
                    const QString fname = rec.fieldName(i);
                    if (fname == "id")
                        continue;
                    sets << QString("%1=?").arg(fname);
                }
                QSqlQuery upd(remote);
                upd.prepare(QString("UPDATE %1 SET %2 WHERE id=?")
                            .arg(table)
                            .arg(sets.join(',')));
                for (int i = 0; i < rec.count(); ++i) {
                    if (rec.fieldName(i) == "id")
                        continue;
                    upd.addBindValue(local.value(i));
                }
                upd.addBindValue(local.value("id"));
                if (!upd.exec()) {
                    m_lastError = upd.lastError().text();
                    remote.close();
                    QSqlDatabase::removeDatabase("sync");
                    return false;
                }
            } else {
                QStringList cols, placeholders;
                for (int i = 0; i < rec.count(); ++i) {
                    cols << rec.fieldName(i);
                    placeholders << "?";
                }
                QSqlQuery ins(remote);
                ins.prepare(QString("INSERT INTO %1(%2) VALUES(%3)")
                            .arg(table)
                            .arg(cols.join(','))
                            .arg(placeholders.join(',')));
                for (int i = 0; i < rec.count(); ++i)
                    ins.addBindValue(local.value(i));
                if (!ins.exec()) {
                    m_lastError = ins.lastError().text();
                    remote.close();
                    QSqlDatabase::removeDatabase("sync");
                    return false;
                }
            }
        }
    }

    // Download remote changes
    for (const QString &table : tables) {
        QSqlQuery remoteSel(remote);
        if (!remoteSel.exec(QString("SELECT * FROM %1").arg(table)))
            continue;
        QSqlRecord rec = remoteSel.record();
        while (remoteSel.next()) {
            QVariant idVal = remoteSel.value("id");
            QSqlQuery checkLocal(m_db);
            checkLocal.prepare(QString("SELECT * FROM %1 WHERE id=?").arg(table));
            checkLocal.addBindValue(idVal);
            bool exists = checkLocal.exec() && checkLocal.next();

            QString tsCol;
            if (rec.indexOf("updated_at") != -1)
                tsCol = "updated_at";
            else if (rec.indexOf("last_update") != -1)
                tsCol = "last_update";
            else if (rec.indexOf("created_at") != -1)
                tsCol = "created_at";

            bool applyUpdate = true;
            if (exists && !tsCol.isEmpty()) {
                QDateTime localTs = QDateTime::fromString(checkLocal.value(tsCol).toString(), Qt::ISODate);
                if (!localTs.isValid())
                    localTs = QDateTime::fromString(checkLocal.value(tsCol).toString(), "yyyy-MM-dd HH:mm:ss");
                QDateTime remoteTs = QDateTime::fromString(remoteSel.value(tsCol).toString(), Qt::ISODate);
                if (!remoteTs.isValid())
                    remoteTs = QDateTime::fromString(remoteSel.value(tsCol).toString(), "yyyy-MM-dd HH:mm:ss");
                if (localTs >= remoteTs)
                    applyUpdate = false;
            } else if (exists && tsCol.isEmpty()) {
                applyUpdate = false; // no timestamp to compare, keep local
            }

            if (!exists) {
                QStringList cols, placeholders;
                for (int i = 0; i < rec.count(); ++i) {
                    cols << rec.fieldName(i);
                    placeholders << "?";
                }
                QSqlQuery ins(m_db);
                ins.prepare(QString("INSERT INTO %1(%2) VALUES(%3)")
                            .arg(table)
                            .arg(cols.join(','))
                            .arg(placeholders.join(',')));
                for (int i = 0; i < rec.count(); ++i)
                    ins.addBindValue(remoteSel.value(i));
                if (!ins.exec()) {
                    m_lastError = ins.lastError().text();
                    remote.close();
                    QSqlDatabase::removeDatabase("sync");
                    return false;
                }
            } else if (applyUpdate) {
                QStringList sets;
                for (int i = 0; i < rec.count(); ++i) {
                    const QString fname = rec.fieldName(i);
                    if (fname == "id")
                        continue;
                    sets << QString("%1=?").arg(fname);
                }
                QSqlQuery upd(m_db);
                upd.prepare(QString("UPDATE %1 SET %2 WHERE id=?")
                            .arg(table)
                            .arg(sets.join(',')));
                for (int i = 0; i < rec.count(); ++i) {
                    if (rec.fieldName(i) == "id")
                        continue;
                    upd.addBindValue(remoteSel.value(i));
                }
                upd.addBindValue(idVal);
                if (!upd.exec()) {
                    m_lastError = upd.lastError().text();
                    remote.close();
                    QSqlDatabase::removeDatabase("sync");
                    return false;
                }
            }
        }
    }

    remote.close();
    QSqlDatabase::removeDatabase("sync");
    return true;
}

QString DatabaseManager::lastError() const
{
    return m_lastError;
}
