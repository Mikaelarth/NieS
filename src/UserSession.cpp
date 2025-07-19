#include "UserSession.h"
#include "UserManager.h"

UserSession::UserSession(UserManager *manager, QObject *parent)
    : QObject(parent), m_userManager(manager)
{
}

bool UserSession::login(const QString &username, const QString &password)
{
    if (!m_userManager)
        return false;
    if (!m_userManager->authenticate(username, password))
        return false;
    m_user = m_userManager->currentUser();
    m_role = m_userManager->currentRole();
    emit loggedIn(m_user);
    return true;
}

void UserSession::logout()
{
    if (m_userManager)
        m_userManager->logout();
    if (!m_user.isEmpty())
        emit loggedOut();
    m_user.clear();
    m_role.clear();
}

QString UserSession::user() const
{
    return m_user;
}

QString UserSession::role() const
{
    return m_role;
}

bool UserSession::hasRole(const QString &role) const
{
    return m_role == role;
}

bool UserSession::hasAnyRole(const QStringList &roles) const
{
    return roles.contains(m_role);
}

QString UserSession::lastError() const
{
    return m_userManager ? m_userManager->lastError() : QString();
}
