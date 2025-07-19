#include <QtTest>
#include <QCoreApplication>
#include "DatabaseManager.h"
#include "UserManager.h"
#include "ProductManager.h"
#include "SalesManager.h"
#include <QTemporaryDir>
#include <QProcess>
#include <QRandomGenerator>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QFile>
#include <QTextStream>

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
    void configPathEnv();
    void openSuccess();
    void crudOperations();
};

static bool startPostgres(QTemporaryDir &dir, int port)
{
    QString initdb = QString::fromLocal8Bit(qgetenv("NIES_INITDB_PATH"));
    if (initdb.isEmpty())
        initdb = "/usr/lib/postgresql/16/bin/initdb";
    QString pgctl = QString::fromLocal8Bit(qgetenv("NIES_PG_CTL_PATH"));
    if (pgctl.isEmpty())
        pgctl = "/usr/lib/postgresql/16/bin/pg_ctl";

    QProcess::execute("chown", {"-R", "postgres:postgres", dir.path()});

    QStringList initArgs{"-u", "postgres", "--", initdb, "-D", dir.path(), "-A", "trust", "-U", "postgres", "--nosync"};
    if (QProcess::execute("runuser", initArgs) != 0)
        return false;

    QStringList startArgs{"-u", "postgres", "--", pgctl, "-D", dir.path(), "-o", QString("-F -p %1").arg(port), "-w", "start"};
    return QProcess::execute("runuser", startArgs) == 0;
}

static void stopPostgres(const QString &dir)
{
    QString pgctl = QString::fromLocal8Bit(qgetenv("NIES_PG_CTL_PATH"));
    if (pgctl.isEmpty())
        pgctl = "/usr/lib/postgresql/16/bin/pg_ctl";
    QStringList stopArgs{"-u", "postgres", "--", pgctl, "-D", dir, "-m", "fast", "-w", "stop"};
    QProcess::execute("runuser", stopArgs);
}

void PostgresTest::configPathEnv()
{
    if (!qEnvironmentVariableIsEmpty("NIES_SKIP_PG_TESTS"))
        QSKIP("PostgreSQL tests skipped because NIES_SKIP_PG_TESTS is set");
    QTemporaryDir dbDir;
    QVERIFY(dbDir.isValid());
    int port = 54321 + static_cast<int>(QRandomGenerator::global()->bounded(1000));
    QVERIFY(startPostgres(dbDir, port));

    QTemporaryDir confDir;
    QVERIFY(confDir.isValid());
    QString confPath = confDir.filePath("test.ini");
    QFile file(confPath);
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream ts(&file);
    ts << "[database]\n";
    ts << "host=localhost\n";
    ts << "port=" << port << "\n";
    ts << "name=postgres\n";
    ts << "user=postgres\n";
    ts << "password=\n";
    file.close();

    qunsetenv("NIES_DB_HOST");
    qunsetenv("NIES_DB_PORT");
    qunsetenv("NIES_DB_NAME");
    qunsetenv("NIES_DB_USER");
    qunsetenv("NIES_DB_PASSWORD");

    qputenv("NIES_CONFIG_PATH", confPath.toLocal8Bit());

    DatabaseManager db;
    QVERIFY(db.open());
    db.close();

    qunsetenv("NIES_CONFIG_PATH");
    stopPostgres(dbDir.path());
}

void PostgresTest::openSuccess()
{
    if (!qEnvironmentVariableIsEmpty("NIES_SKIP_PG_TESTS"))
        QSKIP("PostgreSQL tests skipped because NIES_SKIP_PG_TESTS is set");
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
    if (!qEnvironmentVariableIsEmpty("NIES_SKIP_PG_TESTS"))
        QSKIP("PostgreSQL tests skipped because NIES_SKIP_PG_TESTS is set");
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

class UserManagerTest : public QObject
{
    Q_OBJECT
private slots:
    void createUser();
};

void UserManagerTest::createUser()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    QSqlQuery query;
    QVERIFY(query.exec("CREATE TABLE users("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "username TEXT UNIQUE,"
                       "password_hash TEXT,"
                       "role TEXT,"
                       "created_at TEXT)"));

    UserManager um;
    QVERIFY(um.createUser("alice", "hash", "admin"));

    QVERIFY(query.exec("SELECT username, role FROM users WHERE username='alice'"));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toString(), QString("alice"));
    QCOMPARE(query.value(1).toString(), QString("admin"));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

class ProductManagerTest : public QObject
{
    Q_OBJECT
private slots:
    void addProduct();
};

void ProductManagerTest::addProduct()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    QSqlQuery query;
    QVERIFY(query.exec("CREATE TABLE products("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "name TEXT,"
                       "price REAL,"
                       "discount REAL DEFAULT 0,"
                       "created_at TEXT,"
                       "updated_at TEXT)"));

    ProductManager pm;
    QVERIFY(pm.addProduct("Widget", 9.99, 1.0));

    QVERIFY(query.exec("SELECT name, price, discount FROM products WHERE id=1"));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toString(), QString("Widget"));
    QCOMPARE(query.value(1).toDouble(), 9.99);
    QCOMPARE(query.value(2).toDouble(), 1.0);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

class SalesManagerTest : public QObject
{
    Q_OBJECT
private slots:
    void recordSaleUpdatesInventory();
};

void SalesManagerTest::recordSaleUpdatesInventory()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    QSqlQuery query;
    QVERIFY(query.exec("CREATE TABLE products("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "name TEXT,"
                       "price REAL,"
                       "discount REAL DEFAULT 0,"
                       "created_at TEXT,"
                       "updated_at TEXT)"));
    QVERIFY(query.exec("CREATE TABLE inventory("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "product_id INTEGER,"
                       "quantity INTEGER,"
                       "last_update TEXT)"));
    QVERIFY(query.exec("CREATE TABLE sales("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "product_id INTEGER,"
                       "quantity INTEGER,"
                       "sale_date TEXT,"
                       "total REAL)"));

    QVERIFY(query.exec("INSERT INTO products(name, price, discount) VALUES('Thing', 10.0, 0.1)"));
    int productId = query.lastInsertId().toInt();
    QVERIFY(query.exec(QString("INSERT INTO inventory(product_id, quantity) VALUES(%1, 10)").arg(productId)));

    SalesManager sm;
    QVERIFY2(sm.recordSale(productId, 3), qPrintable(sm.lastError()));

    QVERIFY(query.exec("SELECT quantity, total FROM sales WHERE id=1"));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toInt(), 3);
    QCOMPARE(query.value(1).toDouble(), 27.0);

    QVERIFY(query.exec(QString("SELECT quantity FROM inventory WHERE product_id=%1").arg(productId)));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toInt(), 7);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
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
    UserManagerTest userTest;
    status |= QTest::qExec(&userTest, argc, argv);
    ProductManagerTest prodTest;
    status |= QTest::qExec(&prodTest, argc, argv);
    SalesManagerTest salesTest;
    status |= QTest::qExec(&salesTest, argc, argv);
    return status;
}

#include "database_test.moc"

