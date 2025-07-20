#include <QtTest>
#include "employees/EmployeeManager.h"
#include "employee_manager_test.h"

void EmployeeManagerTest::payrollZeroByDefault()
{
    EmployeeManager mgr;
    QCOMPARE(mgr.payroll(1), 0.0);
}

void EmployeeManagerTest::schedulingAddsShift()
{
    EmployeeManager mgr;
    QDateTime start = QDateTime::currentDateTime();
    QDateTime end = start.addSecs(3600);
    QVERIFY(mgr.scheduleShift(1, start, end));
    QCOMPARE(mgr.shifts(1).size(), 1);
}

