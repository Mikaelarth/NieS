#include <QtTest>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "login/MainWindow.h"
#include "UserManager.h"
#include "UserSession.h"
#include "main_window_test.h"
#include <QAction>
#include "dashboard/DashboardWindow.h"
#include "stock/StockPredictionWindow.h"
#include "employees/EmployeeWindow.h"

static void setupUsersTable()
{
    QSqlQuery query;
    QVERIFY(query.exec("CREATE TABLE users("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "username TEXT UNIQUE,"
                       "password_hash TEXT,"
                       "password_salt TEXT,"
                       "role TEXT,"
                       "created_at TEXT)"));
}

class ScopedDb
{
public:
    ScopedDb()
    {
        if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
            QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(":memory:");
        QVERIFY(db.open());
        setupUsersTable();
    }
    ~ScopedDb()
    {
        db.close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }
    QSqlDatabase db;
};

void MainWindowTest::adminFullAccess()
{
    ScopedDb scoped;
    UserManager um;
    UserSession session(&um, &um);
    QVERIFY(um.createUser("admin", "pw", "admin"));
    QVERIFY(session.login("admin", "pw"));

    MainWindow win(&session);
    win.show();
    QVERIFY(QTest::qWaitForWindowExposed(&win));

    QAction *manageAct = win.findChild<QAction*>("manageAct");
    QAction *posAct = win.findChild<QAction*>("posAct");
    QAction *empAct = win.findChild<QAction*>("employeesAct");
    QVERIFY(manageAct && posAct && empAct);
    QVERIFY(manageAct->isEnabled());
    QVERIFY(posAct->isEnabled());
    QVERIFY(empAct->isEnabled());
}

void MainWindowTest::sellerLimitedAccess()
{
    ScopedDb scoped;
    UserManager um;
    UserSession session(&um, &um);
    QVERIFY(um.createUser("sell", "pw", "seller"));
    QVERIFY(session.login("sell", "pw"));

    MainWindow win(&session);
    win.show();
    QVERIFY(QTest::qWaitForWindowExposed(&win));

    QAction *manageAct = win.findChild<QAction*>("manageAct");
    QAction *posAct = win.findChild<QAction*>("posAct");
    QAction *empAct = win.findChild<QAction*>("employeesAct");
    QVERIFY(manageAct && posAct && empAct);
    QVERIFY(!manageAct->isEnabled());
    QVERIFY(posAct->isEnabled());
    QVERIFY(!empAct->isEnabled());
}

void MainWindowTest::viewerNoAccess()
{
    ScopedDb scoped;
    UserManager um;
    UserSession session(&um, &um);
    QVERIFY(um.createUser("view", "pw", "viewer"));
    QVERIFY(session.login("view", "pw"));

    MainWindow win(&session);
    win.show();
    QVERIFY(QTest::qWaitForWindowExposed(&win));

    QAction *manageAct = win.findChild<QAction*>("manageAct");
    QAction *posAct = win.findChild<QAction*>("posAct");
    QAction *empAct = win.findChild<QAction*>("employeesAct");
    QVERIFY(manageAct && posAct && empAct);
    QVERIFY(!manageAct->isEnabled());
    QVERIFY(!posAct->isEnabled());
    QVERIFY(!empAct->isEnabled());
}

void MainWindowTest::dashboardActionOpens()
{
    ScopedDb scoped;
    UserManager um;
    UserSession session(&um, &um);
    QVERIFY(um.createUser("dash", "pw", "seller"));
    QVERIFY(session.login("dash", "pw"));

    MainWindow win(&session);
    win.show();
    QVERIFY(QTest::qWaitForWindowExposed(&win));

    QAction *dashAct = win.findChild<QAction*>("dashboardAct");
    QVERIFY(dashAct);
    QVERIFY(dashAct->isEnabled());
    QVERIFY(!win.findChild<DashboardWindow*>());
    dashAct->trigger();
    DashboardWindow *dash = win.findChild<DashboardWindow*>();
    QVERIFY(dash && dash->isVisible());
}

void MainWindowTest::predictionActionOpens()
{
    ScopedDb scoped;
    UserManager um;
    UserSession session(&um, &um);
    QVERIFY(um.createUser("pred", "pw", "seller"));
    QVERIFY(session.login("pred", "pw"));

    MainWindow win(&session);
    win.show();
    QVERIFY(QTest::qWaitForWindowExposed(&win));

    QAction *predAct = win.findChild<QAction*>("predictAct");
    QVERIFY(predAct);
    QVERIFY(predAct->isEnabled());
    QVERIFY(!win.findChild<StockPredictionWindow*>());
    predAct->trigger();
    StockPredictionWindow *predWin = win.findChild<StockPredictionWindow*>();
    QVERIFY(predWin && predWin->isVisible());
}

void MainWindowTest::employeesActionOpens()
{
    ScopedDb scoped;
    UserManager um;
    UserSession session(&um, &um);
    QVERIFY(um.createUser("emp", "pw", "admin"));
    QVERIFY(session.login("emp", "pw"));

    MainWindow win(&session);
    win.show();
    QVERIFY(QTest::qWaitForWindowExposed(&win));

    QAction *empAct = win.findChild<QAction*>("employeesAct");
    QVERIFY(empAct);
    QVERIFY(empAct->isEnabled());
    QVERIFY(!win.findChild<EmployeeWindow*>());
    empAct->trigger();
    EmployeeWindow *empWin = win.findChild<EmployeeWindow*>();
    QVERIFY(empWin && empWin->isVisible());
}


