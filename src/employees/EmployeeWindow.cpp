#include "employees/EmployeeWindow.h"
#include "employees/EmployeeManager.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QMessageBox>

EmployeeWindow::EmployeeWindow(EmployeeManager *em, QWidget *parent)
    : QWidget(parent), m_em(em)
{
    setWindowTitle(tr("Employees"));

    m_table = new QTableWidget(this);
    m_table->setObjectName("employeeTable");
    m_table->setColumnCount(3);
    m_table->setHorizontalHeaderLabels({tr("ID"), tr("Rate"), tr("Hours")});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(m_table, &QTableWidget::itemSelectionChanged, this, &EmployeeWindow::onRowSelected);

    m_idEdit = new QSpinBox(this);
    m_idEdit->setObjectName("idEdit");
    m_idEdit->setMaximum(999999);

    m_rateEdit = new QDoubleSpinBox(this);
    m_rateEdit->setObjectName("rateEdit");
    m_rateEdit->setMaximum(100000);

    m_hoursEdit = new QDoubleSpinBox(this);
    m_hoursEdit->setObjectName("hoursEdit");
    m_hoursEdit->setMaximum(100000);

    m_startEdit = new QDateTimeEdit(this);
    m_startEdit->setObjectName("startEdit");
    m_startEdit->setDisplayFormat("yyyy-MM-dd HH:mm");

    m_endEdit = new QDateTimeEdit(this);
    m_endEdit->setObjectName("endEdit");
    m_endEdit->setDisplayFormat("yyyy-MM-dd HH:mm");

    m_setRateBtn = new QPushButton(tr("Set Rate"), this);
    m_recordBtn = new QPushButton(tr("Record Hours"), this);
    m_scheduleBtn = new QPushButton(tr("Schedule Shift"), this);

    connect(m_setRateBtn, &QPushButton::clicked, this, &EmployeeWindow::onSetRate);
    connect(m_recordBtn, &QPushButton::clicked, this, &EmployeeWindow::onRecord);
    connect(m_scheduleBtn, &QPushButton::clicked, this, &EmployeeWindow::onSchedule);

    QFormLayout *form = new QFormLayout;
    form->addRow(tr("ID"), m_idEdit);
    form->addRow(tr("Rate"), m_rateEdit);
    form->addRow(tr("Hours"), m_hoursEdit);
    form->addRow(tr("Start"), m_startEdit);
    form->addRow(tr("End"), m_endEdit);

    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->addWidget(m_setRateBtn);
    buttons->addWidget(m_recordBtn);
    buttons->addWidget(m_scheduleBtn);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_table);
    layout->addLayout(form);
    layout->addLayout(buttons);

    loadEmployees();
}

void EmployeeWindow::loadEmployees()
{
    m_table->setRowCount(0);
    if (!m_em)
        return;
    const QList<QVariantMap> employees = m_em->listEmployees();
    for (int i = 0; i < employees.size(); ++i) {
        const QVariantMap &e = employees[i];
        m_table->insertRow(i);
        m_table->setItem(i, 0, new QTableWidgetItem(e.value("id").toString()));
        m_table->setItem(i, 1, new QTableWidgetItem(QString::number(e.value("hourly_rate").toDouble())));
        m_table->setItem(i, 2, new QTableWidgetItem(QString::number(e.value("hours_worked").toDouble())));
    }
    if (m_table->rowCount() > 0)
        m_table->selectRow(0);
}

void EmployeeWindow::onSetRate()
{
    if (!m_em)
        return;
    m_em->setHourlyRate(m_idEdit->value(), m_rateEdit->value());
    loadEmployees();
}

void EmployeeWindow::onRecord()
{
    if (!m_em)
        return;
    if (!m_em->recordWork(m_idEdit->value(), m_hoursEdit->value())) {
        QMessageBox::warning(this, tr("Error"), m_em->lastError());
        return;
    }
    loadEmployees();
}

void EmployeeWindow::onSchedule()
{
    if (!m_em)
        return;
    if (!m_em->scheduleShift(m_idEdit->value(), m_startEdit->dateTime(), m_endEdit->dateTime())) {
        QMessageBox::warning(this, tr("Error"), m_em->lastError());
        return;
    }
    loadEmployees();
}

void EmployeeWindow::onRowSelected()
{
    int row = m_table->currentRow();
    if (row < 0)
        return;
    m_idEdit->setValue(m_table->item(row, 0)->text().toInt());
    m_rateEdit->setValue(m_table->item(row, 1)->text().toDouble());
    m_hoursEdit->setValue(0);
}

