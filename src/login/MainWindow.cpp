#include "MainWindow.h"
#include "products/ProductWindow.h"
#include "sales/POSWindow.h"
#include "ProductManager.h"
#include "SalesManager.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("NieS"));

    QMenu *prodMenu = menuBar()->addMenu(tr("Products"));
    QAction *manageAct = prodMenu->addAction(tr("Manage Products"));
    connect(manageAct, &QAction::triggered, this, &MainWindow::openProducts);

    QMenu *salesMenu = menuBar()->addMenu(tr("Sales"));
    QAction *posAct = salesMenu->addAction(tr("Point of Sale"));
    connect(posAct, &QAction::triggered, this, &MainWindow::openPOS);
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

