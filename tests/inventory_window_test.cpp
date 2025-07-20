#include <QtTest>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSpinBox>

#include "inventory/InventoryWindow.h"
#include "InventoryManager.h"
#include "ProductManager.h"
#include "barcode/BarcodeScanner.h"
#include "inventory_window_test.h"

void InventoryWindowTest::scanAdjustsQuantity()
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
                       "price REAL,"\
                       "discount REAL DEFAULT 0,"\
                       "barcode TEXT,"\
                       "created_at TEXT,"\
                       "updated_at TEXT)"));
    QVERIFY(query.exec("CREATE TABLE inventory("\
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
                       "product_id INTEGER,"\
                       "quantity INTEGER,"\
                       "last_update TEXT)"));

    QVERIFY(query.exec("INSERT INTO products(name, price, discount, barcode) VALUES('Item', 1.0, 0.0, 'abc')"));
    int productId = query.lastInsertId().toInt();

    ProductManager pm;
    InventoryManager im;
    BarcodeScanner scanner;
    InventoryWindow w(&pm, &im, &scanner);
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    scanner.scan("abc");

    auto qtySpin = w.findChild<QSpinBox*>("qtySpin");
    QVERIFY(qtySpin);
    QCOMPARE(qtySpin->value(), 2);

    QVERIFY(QMetaObject::invokeMethod(&w, "onAdd", Qt::DirectConnection));

    QVERIFY(query.exec(QString("SELECT quantity FROM inventory WHERE product_id=%1").arg(productId)));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toInt(), 2);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}


