#ifndef EMPLOYEE_MANAGER_TEST_H
#define EMPLOYEE_MANAGER_TEST_H

#include <QObject>

class EmployeeManagerTest : public QObject
{
    Q_OBJECT
private slots:
    void payrollZeroByDefault();
    void schedulingAddsShift();
    void recordWorkPersists();
};

#endif // EMPLOYEE_MANAGER_TEST_H
