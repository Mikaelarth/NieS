#include "returns/ReturnManager.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariant>

ReturnManager::ReturnManager(QObject *parent)
    : QObject(parent)
{
}

bool ReturnManager::recordReturn(int saleId, int quantity)
{
    QSqlQuery saleQuery;
    saleQuery.prepare("SELECT product_id FROM sales WHERE id = :id");
    saleQuery.bindValue(":id", saleId);
    if (!saleQuery.exec() || !saleQuery.next()) {
        m_lastError = saleQuery.lastError().text();
        if (m_lastError.isEmpty())
            m_lastError = QStringLiteral("Sale not found");
        return false;
    }
    int productId = saleQuery.value(0).toInt();

    QSqlQuery query;
    query.prepare("INSERT INTO returns(sale_id, quantity) VALUES(:sale_id, :qty)");
    query.bindValue(":sale_id", saleId);
    query.bindValue(":qty", quantity);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    // Add stock back
    QSqlQuery inv;
    inv.prepare("UPDATE inventory SET quantity = quantity + :q WHERE product_id = :pid");
    inv.bindValue(":q", quantity);
    inv.bindValue(":pid", productId);
    if (!inv.exec()) {
        m_lastError = inv.lastError().text();
        return false;
    }

    return true;
}

QString ReturnManager::lastError() const
{
    return m_lastError;
}

