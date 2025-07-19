#include "UserManager.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariant>
#include <QCryptographicHash>
#include <QUuid>

UserManager::UserManager(QObject *parent)
    : QObject(parent)
{
}

bool UserManager::createUser(const QString &username, const QString &password, const QString &role)
{
    const QString salt = QUuid::createUuid().toString(QUuid::WithoutBraces);
    const QByteArray combined = (salt + password).toUtf8();
    const QString hash = QCryptographicHash::hash(combined, QCryptographicHash::Sha256).toHex();

    QSqlQuery query;
    query.prepare("INSERT INTO users(username, password_hash, password_salt, role) VALUES(:username, :password_hash, :password_salt, :role)");
    query.bindValue(":username", username);
    query.bindValue(":password_hash", hash);
    query.bindValue(":password_salt", salt);
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

bool UserManager::authenticate(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT password_hash, password_salt FROM users WHERE username = :username");
    query.bindValue(":username", username);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    if (!query.next()) {
        m_lastError = QStringLiteral("User not found");
        return false;
    }

    const QString storedHash = query.value(0).toString();
    const QString salt = query.value(1).toString();
    const QByteArray computed = QCryptographicHash::hash((salt + password).toUtf8(), QCryptographicHash::Sha256).toHex();
    return storedHash == QString(computed);
}

QString UserManager::lastError() const
{
    return m_lastError;
}
