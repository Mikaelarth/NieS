#include <QtTest>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QTemporaryDir>
#include <QFile>

#include "invoices/InvoicePrinter.h"
#include "invoice_printer_test.h"

void InvoicePrinterTest::printCreatesFile()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    QSqlQuery query;
    QVERIFY(query.exec("CREATE TABLE products("\
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
                       "name TEXT,"\
                       "price REAL)"));
    QVERIFY(query.exec("CREATE TABLE sales("\
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
                       "product_id INTEGER,"\
                       "quantity INTEGER,"\
                       "sale_date TEXT,"\
                       "total REAL)"));

    QVERIFY(query.exec("INSERT INTO products(name, price) VALUES('Thing', 2.0)"));
    int productId = query.lastInsertId().toInt();
    QVERIFY(query.exec(QString("INSERT INTO sales(product_id, quantity, sale_date, total) VALUES(%1,3,'2020-01-01',6.0)").arg(productId)));
    int saleId = query.lastInsertId().toInt();

    QTemporaryDir dir;
    QString path = dir.filePath("invoice.txt");

    InvoicePrinter printer;
    QVERIFY2(printer.printInvoice(saleId, path), qPrintable(printer.lastError()));

    QFile f(path);
    QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
    QString contents = QString::fromUtf8(f.readAll());
    QVERIFY(contents.contains(QString("Invoice ID: %1").arg(saleId)));
    QVERIFY(contents.contains("Product: Thing"));
    QVERIFY(contents.contains("Quantity: 3"));
    f.close();

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

