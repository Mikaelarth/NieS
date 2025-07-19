#include <QtTest>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "returns/ReturnManager.h"
#include "return_manager_test.h"

void ReturnManagerTest::recordReturnUpdatesInventory()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    QSqlQuery query;
    QVERIFY(query.exec("CREATE TABLE products("\
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
                       "name TEXT)"));
    QVERIFY(query.exec("CREATE TABLE inventory("\
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
                       "product_id INTEGER,"\
                       "quantity INTEGER,"\
                       "last_update TEXT)"));
    QVERIFY(query.exec("CREATE TABLE sales("\
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
                       "product_id INTEGER,"\
                       "quantity INTEGER,"\
                       "sale_date TEXT,"\
                       "total REAL)"));
    QVERIFY(query.exec("CREATE TABLE returns("\
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
                       "sale_id INTEGER,"\
                       "quantity INTEGER)"));

    QVERIFY(query.exec("INSERT INTO products(name) VALUES('Item')"));
    int productId = query.lastInsertId().toInt();
    QVERIFY(query.exec(QString("INSERT INTO inventory(product_id, quantity) VALUES(%1,5)").arg(productId)));
    QVERIFY(query.exec(QString("INSERT INTO sales(product_id, quantity) VALUES(%1,2)").arg(productId)));
    int saleId = query.lastInsertId().toInt();

    ReturnManager rm;
    QVERIFY2(rm.recordReturn(saleId, 1), qPrintable(rm.lastError()));

    QVERIFY(query.exec(QString("SELECT quantity FROM inventory WHERE product_id=%1").arg(productId)));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toInt(), 6);

    QVERIFY(query.exec(QString("SELECT quantity FROM returns WHERE sale_id=%1").arg(saleId)));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toInt(), 1);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

