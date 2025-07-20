#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVariantMap>

class UserManager : public QObject
{
    Q_OBJECT
public:
    explicit UserManager(QObject *parent = nullptr);

    bool createUser(const QString &username, const QString &password, const QString &role);
    bool deleteUser(const QString &username);
    bool updateUserRole(const QString &username, const QString &newRole);
    bool authenticate(const QString &username, const QString &password);
    QList<QVariantMap> listUsers();

    QString currentUser() const;
    QString currentRole() const;
    void logout();

    QString lastError() const;

private:
    QString m_lastError;
    QString m_currentUser;
    QString m_currentRole;
};

#endif // USERMANAGER_H
