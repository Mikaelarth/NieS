#include "loyalty/LoyaltyManager.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariant>

LoyaltyManager::LoyaltyManager(QObject *parent)
    : QObject(parent)
{
}

bool LoyaltyManager::createAccount(const QString &phone)
{
    QSqlQuery q;
    q.prepare("INSERT INTO loyalty_accounts(phone) VALUES(:phone)");
    q.bindValue(":phone", phone);
    if (!q.exec()) {
        m_lastError = q.lastError().text();
        return false;
    }
    return true;
}

int LoyaltyManager::accountId(const QString &phone) const
{
    QSqlQuery q;
    q.prepare("SELECT id FROM loyalty_accounts WHERE phone=:p");
    q.bindValue(":p", phone);
    if (!q.exec() || !q.next())
        return 0;
    return q.value(0).toInt();
}

bool LoyaltyManager::changePoints(const QString &phone, int delta, const QString &reason)
{
    int id = accountId(phone);
    if (id == 0) {
        if (delta < 0)
            return false;
        if (!createAccount(phone))
            return false;
        id = accountId(phone);
    }

    QSqlQuery bal;
    bal.prepare("SELECT points FROM loyalty_accounts WHERE id=:id");
    bal.bindValue(":id", id);
    if (!bal.exec() || !bal.next())
        return false;
    int points = bal.value(0).toInt() + delta;
    if (points < 0) {
        m_lastError = tr("Insufficient points");
        return false;
    }

    QSqlQuery upd;
    upd.prepare("UPDATE loyalty_accounts SET points=:p WHERE id=:id");
    upd.bindValue(":p", points);
    upd.bindValue(":id", id);
    if (!upd.exec()) {
        m_lastError = upd.lastError().text();
        return false;
    }

    QSqlQuery trans;
    trans.prepare("INSERT INTO loyalty_transactions(account_id, change, reason) VALUES(:id,:ch,:r)");
    trans.bindValue(":id", id);
    trans.bindValue(":ch", delta);
    trans.bindValue(":r", reason);
    trans.exec();

    return true;
}

int LoyaltyManager::points(const QString &phone) const
{
    QSqlQuery q;
    q.prepare("SELECT points FROM loyalty_accounts WHERE phone=:p");
    q.bindValue(":p", phone);
    if (!q.exec() || !q.next())
        return 0;
    return q.value(0).toInt();
}

QList<QVariantMap> LoyaltyManager::listAccounts()
{
    QList<QVariantMap> list;
    QSqlQuery q("SELECT id, phone, points FROM loyalty_accounts ORDER BY id");
    if (!q.exec())
        return list;
    while (q.next()) {
        QVariantMap m;
        m.insert("id", q.value(0));
        m.insert("phone", q.value(1));
        m.insert("points", q.value(2));
        list.append(m);
    }
    return list;
}

QString LoyaltyManager::lastError() const
{
    return m_lastError;
}

