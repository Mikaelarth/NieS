#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ProductWindow;
class POSWindow;
class SalesReportWindow;
class DashboardWindow;
class StockPredictionWindow;
class UserWindow;
class EmployeeWindow;
class LoyaltyWindow;
class UserSession;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(UserSession *session, QWidget *parent = nullptr);

private slots:
    void openProducts();
    void openPOS();
    void openReport();
    void openUsers();
    void openEmployees();
    void openDashboard();
    void openPredictions();
    void openLoyalty();

private:
    void updatePermissions();
    UserSession *m_session;
    QAction *m_manageAct = nullptr;
    QAction *m_posAct = nullptr;
    QAction *m_reportAct = nullptr;
    QAction *m_usersAct = nullptr;
    QAction *m_employeesAct = nullptr;
    QAction *m_dashboardAct = nullptr;
    QAction *m_predictAct = nullptr;
    QAction *m_loyaltyAct = nullptr;
    ProductWindow *m_productWindow = nullptr;
    POSWindow *m_posWindow = nullptr;
    SalesReportWindow *m_reportWindow = nullptr;
    DashboardWindow *m_dashboardWindow = nullptr;
    StockPredictionWindow *m_predictionWindow = nullptr;
    LoyaltyWindow *m_loyaltyWindow = nullptr;
    UserWindow *m_userWindow = nullptr;
    EmployeeWindow *m_employeeWindow = nullptr;
};

#endif // MAINWINDOW_H
