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

/* PostgreSQL specific tests are disabled while the project targets MySQL.
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
    return false;
}

static void stopPostgres(const QString &dir)
{
    Q_UNUSED(dir);
}

void PostgresTest::configPathEnv() {}
void PostgresTest::openSuccess() {}
void PostgresTest::crudOperations() {}
*/

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
    /* Postgres tests disabled */
    UserManagerTest userTest;
    status |= QTest::qExec(&userTest, argc, argv);
    ProductManagerTest prodTest;
    status |= QTest::qExec(&prodTest, argc, argv);
    SalesManagerTest salesTest;
    status |= QTest::qExec(&salesTest, argc, argv);
    return status;
}

#include "database_test.moc"

