#include "ProductManager.h"
#include "UserSession.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariant>

ProductManager::ProductManager(UserSession *session, QObject *parent)
    : QObject(parent), m_session(session)
{
}

bool ProductManager::addProduct(const QString &name, double price, double discount)
{
    if (m_session && !m_session->hasRole("admin")) {
        m_lastError = QStringLiteral("Permission denied");
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO products(name, price, discount) VALUES(:name, :price, :discount)");
    query.bindValue(":name", name);
    query.bindValue(":price", price);
    query.bindValue(":discount", discount);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool ProductManager::updateProduct(int id, const QString &name, double price, double discount)
{
    if (m_session && !m_session->hasRole("admin")) {
        m_lastError = QStringLiteral("Permission denied");
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE products SET name = :name, price = :price, discount = :discount, updated_at = CURRENT_TIMESTAMP WHERE id = :id");
    query.bindValue(":name", name);
    query.bindValue(":price", price);
    query.bindValue(":discount", discount);
    query.bindValue(":id", id);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return query.numRowsAffected() > 0;
}

bool ProductManager::deleteProduct(int id)
{
    if (m_session && !m_session->hasRole("admin")) {
        m_lastError = QStringLiteral("Permission denied");
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM products WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return query.numRowsAffected() > 0;
}

QList<QVariantMap> ProductManager::listProducts()
{
    QList<QVariantMap> products;
    QSqlQuery query("SELECT id, name, price, discount FROM products ORDER BY id");
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return products;
    }
    while (query.next()) {
        QVariantMap product;
        product.insert("id", query.value("id"));
        product.insert("name", query.value("name"));
        product.insert("price", query.value("price"));
        product.insert("discount", query.value("discount"));
        products.append(product);
    }
    return products;
}

QVariantMap ProductManager::productByBarcode(const QString &barcode)
{
    QVariantMap product;
    QSqlQuery query;
    query.prepare("SELECT id, name, price, discount FROM products WHERE barcode = :bc");
    query.bindValue(":bc", barcode);
    if (!query.exec() || !query.next()) {
        m_lastError = query.lastError().text();
        if (m_lastError.isEmpty())
            m_lastError = QStringLiteral("Product not found");
        return product;
    }
    product.insert("id", query.value("id"));
    product.insert("name", query.value("name"));
    product.insert("price", query.value("price"));
    product.insert("discount", query.value("discount"));
    return product;
}

QString ProductManager::lastError() const
{
    return m_lastError;
}

