#include "SalesManager.h"
#include "UserSession.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QVariant>

SalesManager::SalesManager(UserSession *session, QObject *parent)
    : QObject(parent), m_session(session), m_inventory(session, this)
{
}

bool SalesManager::recordSale(int productId, int quantity)
{
    if (m_session && !m_session->hasAnyRole({"seller", "admin"})) {
        m_lastError = QStringLiteral("Permission denied");
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        m_lastError = db.lastError().text();
        return false;
    }

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
        db.rollback();
        return false;
    }

    if (!m_inventory.removeStock(productId, quantity)) {
        m_lastError = m_inventory.lastError();
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        m_lastError = db.lastError().text();
        db.rollback();
        return false;
    }

    emit saleRecorded(productId, quantity);
    return true;
}

QList<QVariantMap> SalesManager::salesReport()
{
    if (m_session && !m_session->hasRole("admin")) {
        m_lastError = QStringLiteral("Permission denied");
        return {};
    }
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

QVariantMap SalesManager::financialReport()
{
    if (m_session && !m_session->hasRole("admin"))
        return {};
    QVariantMap report;
    QSqlQuery q("SELECT SUM(total) as revenue, SUM(quantity) as units FROM sales");
    if (q.exec() && q.next()) {
        report.insert("revenue", q.value(0));
        report.insert("units", q.value(1));
    }
    return report;
}

QString SalesManager::lastError() const
{
    return m_lastError;
}

