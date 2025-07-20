#ifndef EMPLOYEE_WINDOW_TEST_H
#define EMPLOYEE_WINDOW_TEST_H

#include <QObject>

class EmployeeWindowTest : public QObject
{
    Q_OBJECT
private slots:
    void addEmployeeUI();
    void scheduleShiftUI();
};

#endif // EMPLOYEE_WINDOW_TEST_H
