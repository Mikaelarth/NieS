#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSettings>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);

    bool open();
    void close();
    QString lastError() const;

private:
    QSqlDatabase m_db;
    QString m_lastError;
    QSettings m_settings;
};

#endif // DATABASEMANAGER_H
