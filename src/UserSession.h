#ifndef USERSESSION_H
#define USERSESSION_H

#include <QObject>
#include <QString>
#include <QStringList>

class UserManager;

class UserSession : public QObject
{
    Q_OBJECT
public:
    explicit UserSession(UserManager *manager, QObject *parent = nullptr);

    bool login(const QString &username, const QString &password);
    void logout();

    QString user() const;
    QString role() const;
    bool hasRole(const QString &role) const;
    bool hasAnyRole(const QStringList &roles) const;

    QString lastError() const;
    UserManager* manager() const { return m_userManager; }

signals:
    void loggedIn(const QString &username);
    void loggedOut();

private:
    UserManager *m_userManager;
    QString m_user;
    QString m_role;
};

#endif // USERSESSION_H
