#include "MainWindow.h"
#include "products/ProductWindow.h"
#include "ProductManager.h"
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
}

void MainWindow::openProducts()
{
    if (!m_productWindow)
        m_productWindow = new ProductWindow(new ProductManager(this), this);
    m_productWindow->show();
    m_productWindow->raise();
    m_productWindow->activateWindow();
}

