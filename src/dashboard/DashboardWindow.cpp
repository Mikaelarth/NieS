#include "dashboard/DashboardWindow.h"
#include "SalesManager.h"
#include "InventoryManager.h"
#include <QVBoxLayout>
#include <QLabel>

DashboardWindow::DashboardWindow(SalesManager *sm, InventoryManager *im, QWidget *parent)
    : QWidget(parent), m_sm(sm), m_im(im)
{
    setWindowTitle(tr("Dashboard"));
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("Sales and inventory KPIs will be shown here."), this));
}

void DashboardWindow::refresh()
{
    Q_UNUSED(m_sm);
    Q_UNUSED(m_im);
    // Would fetch data and update widgets
}

