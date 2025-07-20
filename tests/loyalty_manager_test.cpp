#include <QtTest>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "loyalty/LoyaltyManager.h"
#include "loyalty_manager_test.h"

void LoyaltyManagerTest::addPointsCreatesAccount()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    QSqlQuery q;
    QVERIFY(q.exec("CREATE TABLE loyalty_accounts("\
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
                   "phone TEXT UNIQUE,"\
                   "points INTEGER DEFAULT 0,"\
                   "created_at TEXT)"));
    QVERIFY(q.exec("CREATE TABLE loyalty_transactions("\
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
                   "account_id INTEGER,"\
                   "change INTEGER,"\
                   "reason TEXT,"\
                   "created_at TEXT)"));

    LoyaltyManager lm;
    QVERIFY(lm.changePoints("123", 5));
    QCOMPARE(lm.points("123"), 5);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

