#include <QtTest>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "login/LoginDialog.h"
#include "UserManager.h"
#include "UserSession.h"
#include "login_test.h"
#include <QLineEdit>
#include <QCheckBox>

void LoginDialogTest::invalidCredentials()
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
    UserSession session(&um, &um);
    QVERIFY(um.createUser("user", "pass", "role"));

    LoginDialog dlg(&session, nullptr, false);
    QVERIFY(!dlg.attemptLogin("user", "wrong"));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void LoginDialogTest::validCredentials()
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
    UserSession session(&um, &um);
    QVERIFY(um.createUser("bob", "secret", "role"));

    LoginDialog dlg(&session, nullptr, false);
    QVERIFY(dlg.attemptLogin("bob", "secret"));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void LoginDialogTest::togglePasswordVisibility()
{
    LoginDialog dlg(nullptr, nullptr, false);
    dlg.show();
    QVERIFY(QTest::qWaitForWindowExposed(&dlg));

    auto passEdit = dlg.findChild<QLineEdit*>("passwordEdit");
    auto check = dlg.findChild<QCheckBox*>("showPassCheck");
    QVERIFY(passEdit && check);
    QCOMPARE(passEdit->echoMode(), QLineEdit::Password);

    check->setChecked(true);
    QCOMPARE(passEdit->echoMode(), QLineEdit::Normal);

    check->setChecked(false);
    QCOMPARE(passEdit->echoMode(), QLineEdit::Password);
}

