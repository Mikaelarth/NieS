#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QSettings>
#include <QString>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(const QString &configPath = QString(),
                             QObject *parent = nullptr);

    bool open();
    bool synchronize();
    void close();
    QString lastError() const;
    bool isOffline() const { return m_offline; }
    QString paymentApiKey() const { return m_paymentApiKey; }
    QString paymentEndpoint() const { return m_paymentEndpoint; }

private:
    QSqlDatabase m_db;
    QString m_lastError;
    QSettings m_settings;
    bool m_offline = false;
    QString m_offlinePath;
    QString m_driver = "QMYSQL";
    QString m_paymentApiKey;
    QString m_paymentEndpoint;
};

#endif // DATABASEMANAGER_H
