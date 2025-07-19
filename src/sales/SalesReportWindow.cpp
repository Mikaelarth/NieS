#include "SalesReportWindow.h"
#include "SalesManager.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>

SalesReportWindow::SalesReportWindow(SalesManager *sm, QWidget *parent)
    : QWidget(parent),
      m_sm(sm)
{
    setWindowTitle(tr("Sales Report"));

    m_table = new QTableWidget(this);
    m_table->setObjectName("salesTable");
    m_table->setColumnCount(5);
    m_table->setHorizontalHeaderLabels({tr("ID"), tr("Product ID"), tr("Quantity"), tr("Date"), tr("Total")});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_table);

    loadReport();
}

void SalesReportWindow::loadReport()
{
    m_table->setRowCount(0);
    if (!m_sm)
        return;
    const QList<QVariantMap> sales = m_sm->salesReport();
    for (int i = 0; i < sales.size(); ++i) {
        const QVariantMap &s = sales[i];
        m_table->insertRow(i);
        m_table->setItem(i, 0, new QTableWidgetItem(s.value("id").toString()));
        m_table->setItem(i, 1, new QTableWidgetItem(s.value("product_id").toString()));
        m_table->setItem(i, 2, new QTableWidgetItem(s.value("quantity").toString()));
        m_table->setItem(i, 3, new QTableWidgetItem(s.value("sale_date").toString()));
        m_table->setItem(i, 4, new QTableWidgetItem(QString::number(s.value("total").toDouble())));
    }
    if (m_table->rowCount() > 0)
        m_table->selectRow(0);
}

