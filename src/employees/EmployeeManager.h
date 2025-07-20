#ifndef EMPLOYEEMANAGER_H
#define EMPLOYEEMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QList>
#include <QVariantMap>
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
    QList<QVariantMap> listEmployees() const;

    QString lastError() const;
private:
    mutable QString m_lastError;
}; 

#endif // EMPLOYEEMANAGER_H
