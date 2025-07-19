#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ProductWindow;
class POSWindow;
class SalesReportWindow;
class UserManager;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(UserManager *userManager, QWidget *parent = nullptr);

private slots:
    void openProducts();
    void openPOS();
    void openReport();

private:
    void updatePermissions();
    UserManager *m_userManager;
    QAction *m_manageAct = nullptr;
    QAction *m_posAct = nullptr;
    QAction *m_reportAct = nullptr;
    ProductWindow *m_productWindow = nullptr;
    POSWindow *m_posWindow = nullptr;
    SalesReportWindow *m_reportWindow = nullptr;
};

#endif // MAINWINDOW_H
