#include "InventoryManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

InventoryManager::InventoryManager(QObject *parent)
    : QObject(parent)
{
}

bool InventoryManager::addStock(int productId, int quantity)
{
    return updateStock(productId, quantity);
}

bool InventoryManager::removeStock(int productId, int quantity)
{
    return updateStock(productId, -quantity);
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
    return true;
}

QString InventoryManager::lastError() const
{
    return m_lastError;
}

