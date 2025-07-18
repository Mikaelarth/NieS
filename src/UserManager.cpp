#include "UserManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

UserManager::UserManager(QObject *parent)
    : QObject(parent)
{
}

bool UserManager::createUser(const QString &username, const QString &passwordHash, const QString &role)
{
    QSqlQuery query;
    query.prepare("INSERT INTO users(username, password_hash, role) VALUES(:username, :password_hash, :role)");
    query.bindValue(":username", username);
    query.bindValue(":password_hash", passwordHash);
    query.bindValue(":role", role);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool UserManager::deleteUser(const QString &username)
{
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE username = :username");
    query.bindValue(":username", username);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return query.numRowsAffected() > 0;
}

bool UserManager::updateUserRole(const QString &username, const QString &newRole)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET role = :role WHERE username = :username");
    query.bindValue(":role", newRole);
    query.bindValue(":username", username);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return query.numRowsAffected() > 0;
}

bool UserManager::authenticate(const QString &username, const QString &passwordHash)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = :username AND password_hash = :password_hash");
    query.bindValue(":username", username);
    query.bindValue(":password_hash", passwordHash);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return query.next();
}

QString UserManager::lastError() const
{
    return m_lastError;
}
