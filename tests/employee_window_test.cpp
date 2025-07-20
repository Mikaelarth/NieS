#include <QtTest>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateTimeEdit>
#include <QTableWidget>

#include "employees/EmployeeWindow.h"
#include "employees/EmployeeManager.h"
#include "employee_window_test.h"

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

void EmployeeWindowTest::addEmployeeUI()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    setupTables();

    EmployeeManager em;
    EmployeeWindow w(&em);
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    auto idEdit = w.findChild<QSpinBox*>("idEdit");
    auto rateEdit = w.findChild<QDoubleSpinBox*>("rateEdit");
    QVERIFY(idEdit && rateEdit);

    idEdit->setValue(1);
    rateEdit->setValue(20.0);

    QVERIFY(QMetaObject::invokeMethod(&w, "onSetRate", Qt::DirectConnection));

    auto table = w.findChild<QTableWidget*>("employeeTable");
    QVERIFY(table);
    QCOMPARE(table->rowCount(), 1);
    QCOMPARE(table->item(0,0)->text(), QString("1"));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void EmployeeWindowTest::scheduleShiftUI()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    setupTables();

    EmployeeManager em;
    em.setHourlyRate(1, 10.0);

    EmployeeWindow w(&em);
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    auto idEdit = w.findChild<QSpinBox*>("idEdit");
    auto startEdit = w.findChild<QDateTimeEdit*>("startEdit");
    auto endEdit = w.findChild<QDateTimeEdit*>("endEdit");
    QVERIFY(idEdit && startEdit && endEdit);

    idEdit->setValue(1);
    QDateTime start = QDateTime::currentDateTime();
    QDateTime end = start.addSecs(3600);
    startEdit->setDateTime(start);
    endEdit->setDateTime(end);

    QVERIFY(QMetaObject::invokeMethod(&w, "onSchedule", Qt::DirectConnection));

    QList<QPair<QDateTime,QDateTime>> shifts = em.shifts(1);
    QCOMPARE(shifts.size(), 1);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

