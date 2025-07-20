#ifndef EMPLOYEEMANAGER_H
#define EMPLOYEEMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QHash>
#include <QList>
#include <utility>

class EmployeeManager : public QObject
{
    Q_OBJECT
public:
    explicit EmployeeManager(QObject *parent = nullptr);

    void setHourlyRate(int employeeId, double rate);
    bool recordWork(int employeeId, double hours);
    bool scheduleShift(int employeeId, const QDateTime &start, const QDateTime &end);

    double payroll(int employeeId) const;
    QList<QPair<QDateTime, QDateTime>> shifts(int employeeId) const;

private:
    struct EmployeeInfo {
        double rate = 0.0;
        double hours = 0.0;
        QList<QPair<QDateTime, QDateTime>> schedule;
    };

    QHash<int, EmployeeInfo> m_data;
};

#endif // EMPLOYEEMANAGER_H
