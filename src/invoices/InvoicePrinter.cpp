#include "invoices/InvoicePrinter.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QFile>
#include <QTextStream>

InvoicePrinter::InvoicePrinter(QObject *parent)
    : QObject(parent)
{
}

bool InvoicePrinter::printInvoice(int saleId, const QString &filePath)
{
    QSqlQuery query;
    query.prepare("SELECT products.name, sales.quantity, sales.total, sales.sale_date "
                  "FROM sales JOIN products ON sales.product_id = products.id "
                  "WHERE sales.id = :id");
    query.bindValue(":id", saleId);
    if (!query.exec() || !query.next()) {
        m_lastError = query.lastError().text();
        if (m_lastError.isEmpty())
            m_lastError = QStringLiteral("Sale not found");
        return false;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_lastError = file.errorString();
        return false;
    }
    QTextStream out(&file);
    out << "Invoice ID: " << saleId << '\n';
    out << "Product: " << query.value(0).toString() << '\n';
    out << "Quantity: " << query.value(1).toInt() << '\n';
    out << "Total: " << query.value(2).toDouble() << '\n';
    out << "Date: " << query.value(3).toString() << '\n';
    file.close();
    return true;
}

QString InvoicePrinter::lastError() const
{
    return m_lastError;
}

