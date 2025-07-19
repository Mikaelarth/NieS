#include "dashboard/DashboardWindow.h"
#include "SalesManager.h"
#include "InventoryManager.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QVariantMap>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>

DashboardWindow::DashboardWindow(SalesManager *sm, InventoryManager *im, QWidget *parent)
    : QWidget(parent), m_sm(sm), m_im(im)
{
    setWindowTitle(tr("Dashboard"));
    auto *layout = new QVBoxLayout(this);

    m_revenueLabel = new QLabel(this);
    m_revenueLabel->setObjectName("revenueLabel");
    layout->addWidget(m_revenueLabel);

    m_unitsLabel = new QLabel(this);
    m_unitsLabel->setObjectName("unitsLabel");
    layout->addWidget(m_unitsLabel);

    m_stockLabel = new QLabel(this);
    m_stockLabel->setObjectName("stockLabel");
    layout->addWidget(m_stockLabel);

    refresh();
}

void DashboardWindow::refresh()
{
    double revenue = 0.0;
    int units = 0;
    if (m_sm) {
        const QVariantMap report = m_sm->financialReport();
        revenue = report.value("revenue").toDouble();
        units = report.value("units").toInt();
    }

    int stock = 0;
    if (m_im) {
        QSqlDatabase db = QSqlDatabase::database();
        if (db.isOpen()) {
            QSqlQuery q(db);
            q.prepare("SELECT SUM(quantity) FROM inventory");
            if (q.exec() && q.next())
                stock = q.value(0).toInt();
        }
    }

    if (m_revenueLabel)
        m_revenueLabel->setText(tr("Total revenue: %1").arg(QString::number(revenue)));
    if (m_unitsLabel)
        m_unitsLabel->setText(tr("Units sold: %1").arg(units));
    if (m_stockLabel)
        m_stockLabel->setText(tr("Stock on hand: %1").arg(stock));
}

