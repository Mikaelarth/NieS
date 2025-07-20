#ifndef EMPLOYEEWINDOW_H
#define EMPLOYEEWINDOW_H

#include <QWidget>

class QTableWidget;
class QSpinBox;
class QDoubleSpinBox;
class QDateTimeEdit;
class QPushButton;
class EmployeeManager;

class EmployeeWindow : public QWidget
{
    Q_OBJECT
public:
    explicit EmployeeWindow(EmployeeManager *em, QWidget *parent = nullptr);

public slots:
    void loadEmployees();

private slots:
    void onSetRate();
    void onRecord();
    void onSchedule();
    void onRowSelected();

private:
    EmployeeManager *m_em;
    QTableWidget *m_table;
    QSpinBox *m_idEdit;
    QDoubleSpinBox *m_rateEdit;
    QDoubleSpinBox *m_hoursEdit;
    QDateTimeEdit *m_startEdit;
    QDateTimeEdit *m_endEdit;
    QPushButton *m_setRateBtn;
    QPushButton *m_recordBtn;
    QPushButton *m_scheduleBtn;
};

#endif // EMPLOYEEWINDOW_H
