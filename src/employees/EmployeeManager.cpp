#include "employees/EmployeeManager.h"

EmployeeManager::EmployeeManager(QObject *parent)
    : QObject(parent)
{
}

void EmployeeManager::setHourlyRate(int employeeId, double rate)
{
    m_data[employeeId].rate = rate;
}

bool EmployeeManager::recordWork(int employeeId, double hours)
{
    if (hours < 0)
        return false;
    m_data[employeeId].hours += hours;
    return true;
}

bool EmployeeManager::scheduleShift(int employeeId, const QDateTime &start, const QDateTime &end)
{
    if (!start.isValid() || !end.isValid() || end <= start)
        return false;
    m_data[employeeId].schedule.append(qMakePair(start, end));
    return true;
}

double EmployeeManager::payroll(int employeeId) const
{
    auto it = m_data.find(employeeId);
    if (it == m_data.end())
        return 0.0;
    return it->hours * it->rate;
}

QList<QPair<QDateTime, QDateTime>> EmployeeManager::shifts(int employeeId) const
{
    return m_data.value(employeeId).schedule;
}

