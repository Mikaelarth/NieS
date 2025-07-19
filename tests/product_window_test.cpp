#include <QtTest>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QTableWidget>

#include "products/ProductWindow.h"
#include "ProductManager.h"
#include "product_window_test.h"

void ProductWindowTest::addProductUI()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    QSqlQuery query;
    QVERIFY(query.exec("CREATE TABLE products("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "name TEXT,"
                       "price REAL,"
                       "discount REAL DEFAULT 0,"
                       "created_at TEXT,"
                       "updated_at TEXT)"));

    ProductManager pm;
    ProductWindow w(&pm);
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    auto nameEdit = w.findChild<QLineEdit*>("nameEdit");
    auto priceEdit = w.findChild<QDoubleSpinBox*>("priceEdit");
    auto discountEdit = w.findChild<QDoubleSpinBox*>("discountEdit");
    QVERIFY(nameEdit && priceEdit && discountEdit);

    nameEdit->setText("TestItem");
    priceEdit->setValue(5.5);
    discountEdit->setValue(0.5);

    QVERIFY(QMetaObject::invokeMethod(&w, "onAdd", Qt::DirectConnection));

    auto table = w.findChild<QTableWidget*>("productTable");
    QVERIFY(table);
    QCOMPARE(table->rowCount(), 1);
    QCOMPARE(table->item(0,1)->text(), QString("TestItem"));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

