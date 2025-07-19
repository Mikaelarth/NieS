#include <QtTest>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "stock/StockPrediction.h"
#include "stock_prediction_test.h"

void StockPredictionTest::predictionAndCritical()
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
                       "sale_date TEXT)"));
    QVERIFY(query.exec("CREATE TABLE inventory("\
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
                       "product_id INTEGER,"\
                       "quantity INTEGER,"\
                       "last_update TEXT)"));

    for (int i=0;i<5;++i) {
        QVERIFY(query.exec(QString("INSERT INTO sales(product_id, quantity, sale_date) VALUES(1,%1,'2020-01-%2')").arg(i+2).arg(i+1)));
    }
    QVERIFY(query.exec("INSERT INTO inventory(product_id, quantity) VALUES(1,4)"));

    StockPrediction sp;
    QCOMPARE(sp.predict(1), 4); // average of 2..6
    QVERIFY(sp.isCritical(1, 10));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

