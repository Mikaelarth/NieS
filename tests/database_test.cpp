#include <QtTest>
#include <QCoreApplication>
#include "DatabaseManager.h"
#include <QTemporaryDir>
#include <QProcess>
#include <QRandomGenerator>
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

class PostgresTest : public QObject
{
    Q_OBJECT
private slots:
    void openSuccess();
    void crudOperations();
};

static bool startPostgres(QTemporaryDir &dir, int port)
{
    QString initdb = "/usr/lib/postgresql/16/bin/initdb";
    QString pgctl = "/usr/lib/postgresql/16/bin/pg_ctl";

    QProcess::execute("chown", {"-R", "postgres:postgres", dir.path()});

    QStringList initArgs{"-u", "postgres", "--", initdb, "-D", dir.path(), "-A", "trust", "-U", "postgres", "--nosync"};
    if (QProcess::execute("runuser", initArgs) != 0)
        return false;

    QStringList startArgs{"-u", "postgres", "--", pgctl, "-D", dir.path(), "-o", QString("-F -p %1").arg(port), "-w", "start"};
    return QProcess::execute("runuser", startArgs) == 0;
}

static void stopPostgres(const QString &dir)
{
    QString pgctl = "/usr/lib/postgresql/16/bin/pg_ctl";
    QStringList stopArgs{"-u", "postgres", "--", pgctl, "-D", dir, "-m", "fast", "-w", "stop"};
    QProcess::execute("runuser", stopArgs);
}

void PostgresTest::openSuccess()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    int port = 55432 + static_cast<int>(QRandomGenerator::global()->bounded(1000));
    QVERIFY(startPostgres(dir, port));

    qputenv("NIES_DB_HOST", QByteArray("localhost"));
    qputenv("NIES_DB_PORT", QByteArray::number(port));
    qputenv("NIES_DB_NAME", QByteArray("postgres"));
    qputenv("NIES_DB_USER", QByteArray("postgres"));
    qputenv("NIES_DB_PASSWORD", QByteArray());

    DatabaseManager db;
    QVERIFY(db.open());
    db.close();

    stopPostgres(dir.path());
}

void PostgresTest::crudOperations()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    int port = 56500 + static_cast<int>(QRandomGenerator::global()->bounded(1000));
    QVERIFY(startPostgres(dir, port));

    qputenv("NIES_DB_HOST", QByteArray("localhost"));
    qputenv("NIES_DB_PORT", QByteArray::number(port));
    qputenv("NIES_DB_NAME", QByteArray("postgres"));
    qputenv("NIES_DB_USER", QByteArray("postgres"));
    qputenv("NIES_DB_PASSWORD", QByteArray());

    DatabaseManager db;
    QVERIFY(db.open());

    QSqlQuery query(QSqlDatabase::database());
    QVERIFY(query.exec("CREATE TABLE IF NOT EXISTS t(id SERIAL PRIMARY KEY, name TEXT);"));
    QVERIFY(query.exec("INSERT INTO t(name) VALUES('Bob');"));
    QVERIFY(query.exec("SELECT name FROM t WHERE id=1;"));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toString(), QString("Bob"));

    db.close();
    stopPostgres(dir.path());
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    int status = 0;
    DatabaseManagerTest dbTest;
    status |= QTest::qExec(&dbTest, argc, argv);
    SQLiteQueryTest queryTest;
    status |= QTest::qExec(&queryTest, argc, argv);
    PostgresTest pgTest;
    status |= QTest::qExec(&pgTest, argc, argv);
    return status;
}

#include "database_test.moc"

