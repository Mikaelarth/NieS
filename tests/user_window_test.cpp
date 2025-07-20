#include <QtTest>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QLineEdit>
#include <QTableWidget>

#include "login/UserWindow.h"
#include "UserManager.h"
#include "user_window_test.h"

void UserWindowTest::createUserUI()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    QSqlQuery query;
    QVERIFY(query.exec(
               "CREATE TABLE users("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "username TEXT UNIQUE,"
               "password_hash TEXT,"
               "password_salt TEXT,"
               "role TEXT,"
               "created_at TEXT)"));

    UserManager um;
    UserWindow w(&um);
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    auto userEdit = w.findChild<QLineEdit*>("userEdit");
    auto passEdit = w.findChild<QLineEdit*>("passEdit");
    auto roleEdit = w.findChild<QLineEdit*>("roleEdit");
    QVERIFY(userEdit && passEdit && roleEdit);

    userEdit->setText("alice");
    passEdit->setText("pw");
    roleEdit->setText("seller");

    QVERIFY(QMetaObject::invokeMethod(&w, "onAdd", Qt::DirectConnection));

    auto table = w.findChild<QTableWidget*>("userTable");
    QVERIFY(table);
    QCOMPARE(table->rowCount(), 1);
    QCOMPARE(table->item(0,0)->text(), QString("alice"));
    QCOMPARE(table->item(0,1)->text(), QString("seller"));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void UserWindowTest::changeRoleUI()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    QSqlQuery query;
    QVERIFY(query.exec(
               "CREATE TABLE users("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "username TEXT UNIQUE,"
               "password_hash TEXT,"
               "password_salt TEXT,"
               "role TEXT,"
               "created_at TEXT)"));
    UserManager um;
    QVERIFY(um.createUser("bob", "pw", "seller"));

    UserWindow w(&um);
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    auto table = w.findChild<QTableWidget*>("userTable");
    QVERIFY(table);
    QVERIFY(table->rowCount() == 1);
    table->selectRow(0);

    auto roleEdit = w.findChild<QLineEdit*>("roleEdit");
    QVERIFY(roleEdit);
    roleEdit->setText("admin");

    QVERIFY(QMetaObject::invokeMethod(&w, "onEdit", Qt::DirectConnection));

    QCOMPARE(table->item(0,1)->text(), QString("admin"));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

