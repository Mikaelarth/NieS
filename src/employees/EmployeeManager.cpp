#include "employees/EmployeeManager.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

EmployeeManager::EmployeeManager(QObject *parent)
    : QObject(parent)
{
}

void EmployeeManager::setHourlyRate(int employeeId, double rate)
{
    QSqlQuery q;
    q.prepare("SELECT id FROM employees WHERE id=?");
    q.addBindValue(employeeId);
    bool exists = q.exec() && q.next();
    QSqlQuery query;
    if (exists) {
        query.prepare("UPDATE employees SET hourly_rate=?, updated_at=CURRENT_TIMESTAMP WHERE id=?");
        query.addBindValue(rate);
        query.addBindValue(employeeId);
    } else {
        query.prepare("INSERT INTO employees(id, hourly_rate, hours_worked) VALUES(?, ?, 0)");
        query.addBindValue(employeeId);
        query.addBindValue(rate);
    }
    if (!query.exec())
        m_lastError = query.lastError().text();
    else
        m_lastError.clear();
}

bool EmployeeManager::recordWork(int employeeId, double hours)
{
    if (hours < 0)
        return false;
    QSqlQuery q;
    q.prepare("SELECT hours_worked FROM employees WHERE id=?");
    q.addBindValue(employeeId);
    if (!q.exec()) {
        m_lastError = q.lastError().text();
        return false;
    }
    double current = 0;
    bool exists = false;
    if (q.next()) {
        current = q.value(0).toDouble();
        exists = true;
    }
    QSqlQuery m;
    if (exists) {
        m.prepare("UPDATE employees SET hours_worked=?, updated_at=CURRENT_TIMESTAMP WHERE id=?");
        m.addBindValue(current + hours);
        m.addBindValue(employeeId);
    } else {
        m.prepare("INSERT INTO employees(id, hourly_rate, hours_worked) VALUES(?, 0, ?)");
        m.addBindValue(employeeId);
        m.addBindValue(hours);
    }
    if (!m.exec()) {
        m_lastError = m.lastError().text();
        return false;
    }
    m_lastError.clear();
    return true;
}

bool EmployeeManager::scheduleShift(int employeeId, const QDateTime &start, const QDateTime &end)
{
    if (!start.isValid() || !end.isValid() || end <= start)
        return false;

    QSqlQuery chk;
    chk.prepare("SELECT id FROM employees WHERE id=?");
    chk.addBindValue(employeeId);
    bool exists = chk.exec() && chk.next();
    if (!exists) {
        QSqlQuery ins;
        ins.prepare("INSERT INTO employees(id, hourly_rate, hours_worked) VALUES(?, 0, 0)");
        ins.addBindValue(employeeId);
        if (!ins.exec()) {
            m_lastError = ins.lastError().text();
            return false;
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO shifts(employee_id, start_time, end_time) VALUES(?, ?, ?)");
    query.addBindValue(employeeId);
    query.addBindValue(start.toString(Qt::ISODate));
    query.addBindValue(end.toString(Qt::ISODate));
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    m_lastError.clear();
    return true;
}

double EmployeeManager::payroll(int employeeId) const
{
    QSqlQuery q;
    q.prepare("SELECT hourly_rate, hours_worked FROM employees WHERE id=?");
    q.addBindValue(employeeId);
    if (!q.exec() || !q.next())
        return 0.0;
    double rate = q.value(0).toDouble();
    double hours = q.value(1).toDouble();
    return rate * hours;
}

QList<QPair<QDateTime, QDateTime>> EmployeeManager::shifts(int employeeId) const
{
    QList<QPair<QDateTime, QDateTime>> list;
    QSqlQuery q;
    q.prepare("SELECT start_time, end_time FROM shifts WHERE employee_id=? ORDER BY start_time");
    q.addBindValue(employeeId);
    if (!q.exec())
        return list;
    while (q.next()) {
        QDateTime s = QDateTime::fromString(q.value(0).toString(), Qt::ISODate);
        if (!s.isValid())
            s = QDateTime::fromString(q.value(0).toString(), "yyyy-MM-dd HH:mm:ss");
        QDateTime e = QDateTime::fromString(q.value(1).toString(), Qt::ISODate);
        if (!e.isValid())
            e = QDateTime::fromString(q.value(1).toString(), "yyyy-MM-dd HH:mm:ss");
        list.append(qMakePair(s, e));
    }
    return list;
}

QList<QVariantMap> EmployeeManager::listEmployees() const
{
    QList<QVariantMap> employees;
    QSqlQuery q("SELECT id, hourly_rate, hours_worked FROM employees ORDER BY id");
    if (!q.exec())
        return employees;
    while (q.next()) {
        QVariantMap e;
        e.insert("id", q.value(0));
        e.insert("hourly_rate", q.value(1));
        e.insert("hours_worked", q.value(2));
        employees.append(e);
    }
    return employees;
}

QString EmployeeManager::lastError() const
{
    return m_lastError;
}
