#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QString>

class UserManager : public QObject
{
    Q_OBJECT
public:
    explicit UserManager(QObject *parent = nullptr);

    bool createUser(const QString &username, const QString &password, const QString &role);
    bool deleteUser(const QString &username);
    bool updateUserRole(const QString &username, const QString &newRole);
    bool authenticate(const QString &username, const QString &password);

    QString lastError() const;

private:
    QString m_lastError;
};

#endif // USERMANAGER_H
