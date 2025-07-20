#include <QtTest>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSpinBox>
#include <QTemporaryDir>
#include <QFile>
#include <QTimer>
#include <QMessageBox>

#include "sales/POSWindow.h"
#include "ProductManager.h"
#include "SalesManager.h"
#include "loyalty/LoyaltyManager.h"
#include "pos_window_test.h"

void POSWindowTest::sellItemUpdatesInventory()
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
                       "created_at TEXT,"\
                       "updated_at TEXT)"));
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

    QVERIFY(query.exec("INSERT INTO products(name, price, discount) VALUES('Item', 2.0, 0.0)"));
    int productId = query.lastInsertId().toInt();
    QVERIFY(query.exec(QString("INSERT INTO inventory(product_id, quantity) VALUES(%1, 5)").arg(productId)));

    ProductManager pm;
    SalesManager sm;
    LoyaltyManager lm;
    POSWindow w(&pm, &sm, &lm);
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    auto qtySpin = w.findChild<QSpinBox*>("qtySpin");
    QVERIFY(qtySpin);
    qtySpin->setValue(3);

    QVERIFY(QMetaObject::invokeMethod(&w, "onSell", Qt::DirectConnection));

    QVERIFY(query.exec(QString("SELECT quantity FROM inventory WHERE product_id=%1").arg(productId)));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toInt(), 2);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

class TestPOSWindow : public POSWindow
{
public:
    QString path;
    TestPOSWindow(ProductManager *pm, SalesManager *sm, LoyaltyManager *lm)
        : POSWindow(pm, sm, lm)
    {}

protected:
    QString askInvoicePath() override { return path; }
};

void POSWindowTest::printInvoiceCreatesFile()
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
    QVERIFY(query.exec(QString("INSERT INTO sales(product_id, quantity, sale_date, total) "
                          "VALUES(%1,1,'2020-01-01',2.0)").arg(productId)));
    int saleId = query.lastInsertId().toInt();

    QTemporaryDir dir;
    QString filePath = dir.filePath("invoice.txt");

    ProductManager pm;
    SalesManager sm;
    LoyaltyManager lm;
    TestPOSWindow w(&pm, &sm, &lm);
    w.path = filePath;
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    QTimer::singleShot(0, []() {
        for (QWidget *w : QApplication::topLevelWidgets()) {
            if (auto mb = qobject_cast<QMessageBox*>(w))
                mb->accept();
        }
    });

    QVERIFY(QMetaObject::invokeMethod(&w, "onPrintInvoice", Qt::DirectConnection));

    QFile f(filePath);
    QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
    QString contents = QString::fromUtf8(f.readAll());
    QVERIFY(contents.contains(QString("Invoice ID: %1").arg(saleId)));
    f.close();

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

