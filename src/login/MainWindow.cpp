#include "MainWindow.h"
#include "products/ProductWindow.h"
#include "sales/POSWindow.h"
#include "sales/SalesReportWindow.h"
#include "login/UserWindow.h"
#include "dashboard/DashboardWindow.h"
#include "UserManager.h"
#include "ProductManager.h"
#include "SalesManager.h"
#include "InventoryManager.h"
#include "UserSession.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(UserSession *session, QWidget *parent)
    : QMainWindow(parent),
      m_session(session)
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

    QMenu *userMenu = menuBar()->addMenu(tr("Users"));
    m_usersAct = userMenu->addAction(tr("Manage Users"));
    m_usersAct->setObjectName("usersAct");
    connect(m_usersAct, &QAction::triggered, this, &MainWindow::openUsers);

    m_dashboardAct = menuBar()->addAction(tr("Dashboard"));
    m_dashboardAct->setObjectName("dashboardAct");
    connect(m_dashboardAct, &QAction::triggered, this, &MainWindow::openDashboard);

    updatePermissions();
}

void MainWindow::openProducts()
{
    if (!m_productWindow)
        m_productWindow = new ProductWindow(new ProductManager(m_session, this), this);
    m_productWindow->show();
    m_productWindow->raise();
    m_productWindow->activateWindow();
}

void MainWindow::openPOS()
{
    if (!m_posWindow)
        m_posWindow = new POSWindow(new ProductManager(m_session, this), new SalesManager(m_session, this), this);
    m_posWindow->show();
    m_posWindow->raise();
    m_posWindow->activateWindow();
}

void MainWindow::openReport()
{
    if (!m_reportWindow)
        m_reportWindow = new SalesReportWindow(new SalesManager(m_session, this), this);
    m_reportWindow->show();
    m_reportWindow->raise();
    m_reportWindow->activateWindow();
}

void MainWindow::openUsers()
{
    if (!m_userWindow)
        m_userWindow = new UserWindow(new UserManager(this), this);
    m_userWindow->show();
    m_userWindow->raise();
    m_userWindow->activateWindow();
}

void MainWindow::openDashboard()
{
    if (!m_dashboardWindow)
        m_dashboardWindow = new DashboardWindow(new SalesManager(m_session, this),
                                               new InventoryManager(m_session, this),
                                               60000, this);
    m_dashboardWindow->show();
    m_dashboardWindow->raise();
    m_dashboardWindow->activateWindow();
}

void MainWindow::updatePermissions()
{
    const QString role = m_session ? m_session->role() : QString();
    if (role != "admin")
        m_manageAct->setEnabled(false);
    if (role != "seller" && role != "admin")
        m_posAct->setEnabled(false);
    if (role != "admin")
        m_reportAct->setEnabled(false);
    if (role != "admin")
        m_usersAct->setEnabled(false);
    m_dashboardAct->setEnabled(!role.isEmpty());
}

