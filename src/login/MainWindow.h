#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ProductWindow;
class POSWindow;
class SalesReportWindow;
class DashboardWindow;
class StockPredictionWindow;
class UserWindow;
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
    void openDashboard();
    void openPredictions();

private:
    void updatePermissions();
    UserSession *m_session;
    QAction *m_manageAct = nullptr;
    QAction *m_posAct = nullptr;
    QAction *m_reportAct = nullptr;
    QAction *m_usersAct = nullptr;
    QAction *m_dashboardAct = nullptr;
    QAction *m_predictAct = nullptr;
    ProductWindow *m_productWindow = nullptr;
    POSWindow *m_posWindow = nullptr;
    SalesReportWindow *m_reportWindow = nullptr;
    DashboardWindow *m_dashboardWindow = nullptr;
    StockPredictionWindow *m_predictionWindow = nullptr;
    UserWindow *m_userWindow = nullptr;
};

#endif // MAINWINDOW_H
