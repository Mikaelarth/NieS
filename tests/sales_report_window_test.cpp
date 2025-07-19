#include <QtTest>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QTableWidget>

#include "sales/SalesReportWindow.h"
#include "SalesManager.h"
#include "sales_report_window_test.h"

void SalesReportWindowTest::loadReport()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    QSqlQuery query;
    QVERIFY(query.exec("CREATE TABLE sales("\
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
                       "product_id INTEGER,"\
                       "quantity INTEGER,"\
                       "sale_date TEXT,"\
                       "total REAL)"));
    QVERIFY(query.exec("INSERT INTO sales(product_id, quantity, sale_date, total) VALUES(1,1,'2020-01-01',5.0)"));

    SalesManager sm;
    SalesReportWindow w(&sm);
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    auto table = w.findChild<QTableWidget*>("salesTable");
    QVERIFY(table);
    QVERIFY(table->rowCount() >= 1);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

