#include <QtTest>
#include <QApplication>
#include <QLabel>

#include "dashboard/DashboardWindow.h"
#include "SalesManager.h"
#include "InventoryManager.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "dashboard_window_test.h"

void DashboardWindowTest::initUi()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    QSqlQuery q;
    QVERIFY(q.exec("CREATE TABLE sales("\
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
                   "product_id INTEGER,"\
                   "quantity INTEGER,"\
                   "sale_date TEXT,"\
                   "total REAL)"));
    QVERIFY(q.exec("CREATE TABLE inventory("\
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
                   "product_id INTEGER,"\
                   "quantity INTEGER)"));
    QVERIFY(q.exec("INSERT INTO sales(product_id, quantity, sale_date, total) "
                   "VALUES(1,2,'2020-01-01',10.5)"));
    QVERIFY(q.exec("INSERT INTO inventory(product_id, quantity) VALUES(1,5)"));

    SalesManager sm;
    InventoryManager im;
    DashboardWindow w(&sm, &im);
    QCOMPARE(w.windowTitle(), QString("Dashboard"));
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));
    w.refresh();

    auto revenue = w.findChild<QLabel*>("revenueLabel");
    auto units = w.findChild<QLabel*>("unitsLabel");
    auto stock = w.findChild<QLabel*>("stockLabel");
    QVERIFY(revenue && units && stock);
    QCOMPARE(revenue->text(), QString("Total revenue: 10.5"));
    QCOMPARE(units->text(), QString("Units sold: 2"));
    QCOMPARE(stock->text(), QString("Stock on hand: 5"));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

