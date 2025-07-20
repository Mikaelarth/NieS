#include <QtTest>
#include "employees/EmployeeManager.h"
#include "employee_manager_test.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

static void setupTables()
{
    QSqlQuery q;
    q.exec("CREATE TABLE employees("\
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
           "hourly_rate REAL DEFAULT 0,"\
           "hours_worked REAL DEFAULT 0,"\
           "created_at TEXT,"\
           "updated_at TEXT)");
    q.exec("CREATE TABLE shifts("\
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
           "employee_id INTEGER,"\
           "start_time TEXT,"\
           "end_time TEXT)");
}

void EmployeeManagerTest::payrollZeroByDefault()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    setupTables();

    EmployeeManager mgr;
    QCOMPARE(mgr.payroll(1), 0.0);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void EmployeeManagerTest::schedulingAddsShift()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    setupTables();

    EmployeeManager mgr;
    QDateTime start = QDateTime::currentDateTime();
    QDateTime end = start.addSecs(3600);
    QVERIFY(mgr.scheduleShift(1, start, end));
    QCOMPARE(mgr.shifts(1).size(), 1);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void EmployeeManagerTest::recordWorkPersists()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    setupTables();

    EmployeeManager mgr;
    mgr.setHourlyRate(1, 10.0);
    QVERIFY(mgr.recordWork(1, 5));
    QCOMPARE(mgr.payroll(1), 50.0);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

