#include "MainWindow.h"
#include "products/ProductWindow.h"
#include "sales/POSWindow.h"
#include "sales/SalesReportWindow.h"
#include "ProductManager.h"
#include "SalesManager.h"
#include "UserManager.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(UserManager *userManager, QWidget *parent)
    : QMainWindow(parent),
      m_userManager(userManager)
{
    setWindowTitle(tr("NieS"));

    QMenu *prodMenu = menuBar()->addMenu(tr("Products"));
    m_manageAct = prodMenu->addAction(tr("Manage Products"));
    m_manageAct->setObjectName("manageAct");
    connect(m_manageAct, &QAction::triggered, this, &MainWindow::openProducts);

    QMenu *salesMenu = menuBar()->addMenu(tr("Sales"));
    m_posAct = salesMenu->addAction(tr("Point of Sale"));
    m_posAct->setObjectName("posAct");
    connect(m_posAct, &QAction::triggered, this, &MainWindow::openPOS);

    m_reportAct = salesMenu->addAction(tr("Sales Report"));
    m_reportAct->setObjectName("reportAct");
    connect(m_reportAct, &QAction::triggered, this, &MainWindow::openReport);

    updatePermissions();
}

void MainWindow::openProducts()
{
    if (!m_productWindow)
        m_productWindow = new ProductWindow(new ProductManager(this), this);
    m_productWindow->show();
    m_productWindow->raise();
    m_productWindow->activateWindow();
}

void MainWindow::openPOS()
{
    if (!m_posWindow)
        m_posWindow = new POSWindow(new ProductManager(this), new SalesManager(this), this);
    m_posWindow->show();
    m_posWindow->raise();
    m_posWindow->activateWindow();
}

void MainWindow::openReport()
{
    if (!m_reportWindow)
        m_reportWindow = new SalesReportWindow(new SalesManager(this), this);
    m_reportWindow->show();
    m_reportWindow->raise();
    m_reportWindow->activateWindow();
}

void MainWindow::updatePermissions()
{
    const QString role = m_userManager ? m_userManager->currentRole() : QString();
    if (role != "admin")
        m_manageAct->setEnabled(false);
    if (role != "seller" && role != "admin")
        m_posAct->setEnabled(false);
    if (role != "admin")
        m_reportAct->setEnabled(false);
}

