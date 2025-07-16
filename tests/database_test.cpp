#include <QtTest>
#include <QCoreApplication>
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlRecord>

class DatabaseManagerTest : public QObject
{
    Q_OBJECT
private slots:
    void connectionFailure();
    void envPasswordOverride();
};

void DatabaseManagerTest::connectionFailure()
{
    qputenv("NIES_DB_HOST", QByteArray());
    qputenv("NIES_DB_PORT", QByteArray());
    qputenv("NIES_DB_NAME", QByteArray());
    qputenv("NIES_DB_USER", QByteArray());
    qputenv("NIES_DB_PASSWORD", QByteArray());
    DatabaseManager db;
    QVERIFY(!db.open());
    QVERIFY(!db.lastError().isEmpty());
    db.close();
}

void DatabaseManagerTest::envPasswordOverride()
{
    qputenv("NIES_DB_PASSWORD", QByteArray("dummy"));
    DatabaseManager db;
    QVERIFY(!db.open());
    QVERIFY(!db.lastError().isEmpty());
    db.close();
    qputenv("NIES_DB_PASSWORD", QByteArray());
}

class SQLiteQueryTest : public QObject
{
    Q_OBJECT
private slots:
    void simpleSelect();
};

void SQLiteQueryTest::simpleSelect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "memdb");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    QSqlQuery query(db);
    QVERIFY(query.exec("CREATE TABLE test(id INTEGER PRIMARY KEY, name TEXT);"));
    QVERIFY(query.exec("INSERT INTO test(name) VALUES('Alice');"));
    QVERIFY(query.exec("SELECT name FROM test WHERE id=1;"));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toString(), QString("Alice"));

    db.close();
    QSqlDatabase::removeDatabase("memdb");
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    int status = 0;
    DatabaseManagerTest dbTest;
    status |= QTest::qExec(&dbTest, argc, argv);
    SQLiteQueryTest queryTest;
    status |= QTest::qExec(&queryTest, argc, argv);
    return status;
}

#include "database_test.moc"

