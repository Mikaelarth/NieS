#include "SalesManager.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariant>

SalesManager::SalesManager(QObject *parent)
    : QObject(parent)
{
}

bool SalesManager::recordSale(int productId, int quantity)
{
    QSqlQuery priceQuery;
    priceQuery.prepare("SELECT price, discount FROM products WHERE id = ?");
    priceQuery.addBindValue(productId);
    if (!priceQuery.exec() || !priceQuery.next()) {
        m_lastError = priceQuery.lastError().text();
        if (m_lastError.isEmpty())
            m_lastError = QStringLiteral("Product not found");
        return false;
    }
    double price = priceQuery.value(0).toDouble();
    double discount = priceQuery.value(1).toDouble();
    double total = price * (1.0 - discount) * quantity;

    QSqlQuery query;
    query.prepare("INSERT INTO sales(product_id, quantity, total) VALUES(?, ?, ?)");
    query.addBindValue(productId);
    query.addBindValue(quantity);
    query.addBindValue(total);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    if (!m_inventory.removeStock(productId, quantity)) {
        m_lastError = m_inventory.lastError();
        return false;
    }

    return true;
}

QList<QVariantMap> SalesManager::salesReport()
{
    QList<QVariantMap> sales;
    QSqlQuery query("SELECT id, product_id, quantity, sale_date, total FROM sales ORDER BY sale_date DESC");
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return sales;
    }
    while (query.next()) {
        QVariantMap sale;
        sale.insert("id", query.value("id"));
        sale.insert("product_id", query.value("product_id"));
        sale.insert("quantity", query.value("quantity"));
        sale.insert("sale_date", query.value("sale_date"));
        sale.insert("total", query.value("total"));
        sales.append(sale);
    }
    return sales;
}

QString SalesManager::lastError() const
{
    return m_lastError;
}

