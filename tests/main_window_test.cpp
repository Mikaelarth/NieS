#include <QtTest>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "login/MainWindow.h"
#include "UserManager.h"
#include "UserSession.h"
#include "main_window_test.h"
#include <QAction>

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
    QVERIFY(manageAct && posAct);
    QVERIFY(manageAct->isEnabled());
    QVERIFY(posAct->isEnabled());
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
    QVERIFY(manageAct && posAct);
    QVERIFY(!manageAct->isEnabled());
    QVERIFY(posAct->isEnabled());
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
    QVERIFY(manageAct && posAct);
    QVERIFY(!manageAct->isEnabled());
    QVERIFY(!posAct->isEnabled());
}


