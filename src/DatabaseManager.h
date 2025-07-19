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
    void close();
    QString lastError() const;

private:
    QSqlDatabase m_db;
    QString m_lastError;
    QSettings m_settings;
};

#endif // DATABASEMANAGER_H
