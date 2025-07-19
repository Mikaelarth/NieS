#include "InventoryManager.h"
#include "UserSession.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariant>

InventoryManager::InventoryManager(UserSession *session, QObject *parent)
    : QObject(parent), m_session(session)
{
}

bool InventoryManager::addStock(int productId, int quantity)
{
    if (m_session && !m_session->hasRole("admin")) {
        m_lastError = QStringLiteral("Permission denied");
        return false;
    }
    return updateStock(productId, quantity);
}

bool InventoryManager::removeStock(int productId, int quantity)
{
    if (m_session && !m_session->hasRole("admin")) {
        m_lastError = QStringLiteral("Permission denied");
        return false;
    }
    return updateStock(productId, -quantity);
}

int InventoryManager::currentStock(int productId) const
{
    QSqlQuery q;
    q.prepare("SELECT quantity FROM inventory WHERE product_id = :pid");
    q.bindValue(":pid", productId);
    if (!q.exec() || !q.next())
        return 0;
    return q.value(0).toInt();
}

bool InventoryManager::isCritical(int productId, int threshold) const
{
    return currentStock(productId) < threshold;
}

bool InventoryManager::updateStock(int productId, int delta)
{
    QSqlQuery query;
    query.prepare("SELECT quantity FROM inventory WHERE product_id = :product_id");
    query.bindValue(":product_id", productId);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    int current = 0;
    bool exists = false;
    if (query.next()) {
        current = query.value(0).toInt();
        exists = true;
    }

    int newQty = current + delta;
    if (newQty < 0) {
        m_lastError = QStringLiteral("Insufficient stock");
        return false;
    }

    QSqlQuery modify;
    if (exists) {
        modify.prepare("UPDATE inventory SET quantity = :qty, last_update = CURRENT_TIMESTAMP WHERE product_id = :product_id");
        modify.bindValue(":qty", newQty);
        modify.bindValue(":product_id", productId);
    } else {
        modify.prepare("INSERT INTO inventory(product_id, quantity) VALUES(:product_id, :qty)");
        modify.bindValue(":product_id", productId);
        modify.bindValue(":qty", newQty);
    }

    if (!modify.exec()) {
        m_lastError = modify.lastError().text();
        return false;
    }

    emit stockChanged(productId, newQty);
    return true;
}

QString InventoryManager::lastError() const
{
    return m_lastError;
}

