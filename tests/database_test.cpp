#include <QtTest>
#include <QApplication>
#include "DatabaseManager.h"
#include "UserManager.h"
#include "ProductManager.h"
#include "SalesManager.h"
#include "login_test.h"
#include "product_window_test.h"
#include "pos_window_test.h"
#include "main_window_test.h"
#include "sales_report_window_test.h"
#include <QTemporaryDir>
#include <QProcess>
#include <QRandomGenerator>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QFile>
#include <QTextStream>
#include <QCryptographicHash>

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
    void authenticateUser();
    void updateUserRole();
    void updateUserRoleNonexistent();
    void deleteUser();
    void deleteUserNonexistent();
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
                       "password_salt TEXT,"
                       "role TEXT,"
                       "created_at TEXT)"));

    UserManager um;
    QVERIFY(um.createUser("alice", "secret", "admin"));

    QVERIFY(query.exec("SELECT username, password_hash, password_salt, role FROM users WHERE username='alice'"));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toString(), QString("alice"));
    QString salt = query.value(2).toString();
    QByteArray expected = QCryptographicHash::hash((salt + "secret").toUtf8(), QCryptographicHash::Sha256).toHex();
    QCOMPARE(query.value(1).toString(), QString(expected));
    QCOMPARE(query.value(3).toString(), QString("admin"));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void UserManagerTest::authenticateUser()
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
                       "password_salt TEXT,"
                       "role TEXT,"
                       "created_at TEXT)"));

    UserManager um;
    QVERIFY(um.createUser("bob", "mypwd", "seller"));

    QVERIFY(um.authenticate("bob", "mypwd"));
    QVERIFY(!um.authenticate("bob", "wrong"));
    QCOMPARE(um.lastError(), QString("Incorrect password"));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void UserManagerTest::deleteUser()
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
                       "password_salt TEXT,"
                       "role TEXT,"
                       "created_at TEXT)"));

    UserManager um;
    QVERIFY(um.createUser("charlie", "pw", "user"));
    QVERIFY(um.deleteUser("charlie"));

    QVERIFY(query.exec("SELECT COUNT(*) FROM users"));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toInt(), 0);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void UserManagerTest::deleteUserNonexistent()
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
                       "password_salt TEXT,"
                       "role TEXT,"
                       "created_at TEXT)"));

    UserManager um;
    QVERIFY(!um.deleteUser("ghost"));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void UserManagerTest::updateUserRole()
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
                       "password_salt TEXT,"
                       "role TEXT,"
                       "created_at TEXT)"));

    UserManager um;
    QVERIFY(um.createUser("dave", "pwd", "user"));
    QVERIFY(um.updateUserRole("dave", "admin"));

    QVERIFY(query.exec("SELECT role FROM users WHERE username='dave'"));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toString(), QString("admin"));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void UserManagerTest::updateUserRoleNonexistent()
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
                       "password_salt TEXT,"
                       "role TEXT,"
                       "created_at TEXT)"));
    UserManager um;
    QVERIFY(!um.updateUserRole("ghost", "admin"));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

class ProductManagerTest : public QObject
{
    Q_OBJECT
private slots:
    void addProduct();
    void updateProduct();
    void deleteProduct();
    void updateProductNonexistent();
    void deleteProductNonexistent();
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

void ProductManagerTest::updateProduct()
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
    QVERIFY(query.exec("INSERT INTO products(name, price, discount) VALUES('Old', 1.0, 0.0)"));
    int productId = query.lastInsertId().toInt();

    ProductManager pm;
    QVERIFY(pm.updateProduct(productId, "New", 2.5, 0.2));

    QVERIFY(query.exec("SELECT name, price, discount FROM products WHERE id=1"));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toString(), QString("New"));
    QCOMPARE(query.value(1).toDouble(), 2.5);
    QCOMPARE(query.value(2).toDouble(), 0.2);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void ProductManagerTest::deleteProduct()
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
    QVERIFY(query.exec("INSERT INTO products(name, price, discount) VALUES('ToDelete', 5.0, 0.0)"));
    int productId = query.lastInsertId().toInt();

    ProductManager pm;
    QVERIFY(pm.deleteProduct(productId));

    QVERIFY(query.exec("SELECT COUNT(*) FROM products"));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toInt(), 0);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void ProductManagerTest::updateProductNonexistent()
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
    QVERIFY(!pm.updateProduct(42, "None", 1.0, 0.0));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void ProductManagerTest::deleteProductNonexistent()
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
    QVERIFY(!pm.deleteProduct(99));

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

class SalesManagerTest : public QObject
{
    Q_OBJECT
private slots:
    void recordSaleUpdatesInventory();
    void recordSaleRollbackOnFailure();
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

void SalesManagerTest::recordSaleRollbackOnFailure()
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

    QVERIFY(query.exec("INSERT INTO products(name, price, discount) VALUES('Item', 2.0, 0.0)"));
    int productId = query.lastInsertId().toInt();
    QVERIFY(query.exec(QString("INSERT INTO inventory(product_id, quantity) VALUES(%1, 1)").arg(productId)));

    SalesManager sm;
    QVERIFY(!sm.recordSale(productId, 5));
    QCOMPARE(sm.lastError(), QString("Insufficient stock"));

    QVERIFY(query.exec("SELECT COUNT(*) FROM sales"));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toInt(), 0);

    QVERIFY(query.exec(QString("SELECT quantity FROM inventory WHERE product_id=%1").arg(productId)));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toInt(), 1);

    db.close();

    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}
class InventoryManagerTest : public QObject
{
    Q_OBJECT
private slots:
    void addStockExistingProduct();
    void removeStockInsufficient();
};

class OfflineSyncTest : public QObject
{
    Q_OBJECT
private slots:
    void syncNewRows();
};

void InventoryManagerTest::addStockExistingProduct()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    QSqlQuery query;
    QVERIFY(query.exec("CREATE TABLE inventory("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "product_id INTEGER,"
                       "quantity INTEGER,"
                       "last_update TEXT)"));
    QVERIFY(query.exec("INSERT INTO inventory(product_id, quantity) VALUES(1, 5)"));

    InventoryManager im;
    QVERIFY2(im.addStock(1, 3), qPrintable(im.lastError()));

    QVERIFY(query.exec("SELECT quantity FROM inventory WHERE product_id = 1"));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toInt(), 8);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void InventoryManagerTest::removeStockInsufficient()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    QVERIFY(db.open());

    QSqlQuery query;
    QVERIFY(query.exec("CREATE TABLE inventory("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "product_id INTEGER,"
                       "quantity INTEGER,"
                       "last_update TEXT)"));
    QVERIFY(query.exec("INSERT INTO inventory(product_id, quantity) VALUES(2, 2)"));

    InventoryManager im;
    QVERIFY(!im.removeStock(2, 5));
    QCOMPARE(im.lastError(), QString("Insufficient stock"));

    QVERIFY(query.exec("SELECT quantity FROM inventory WHERE product_id = 2"));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toInt(), 2);

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void OfflineSyncTest::syncNewRows()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);

    QTemporaryDir dir;
    QString localPath = dir.filePath("local.db");
    QString remotePath = dir.filePath("remote.db");

    qputenv("NIES_DB_OFFLINE", QByteArray("1"));
    qputenv("NIES_DB_OFFLINE_PATH", localPath.toUtf8());
    qputenv("NIES_DB_DRIVER", QByteArray("QSQLITE"));
    qputenv("NIES_DB_NAME", remotePath.toUtf8());

    DatabaseManager db;
    QVERIFY(db.open());
    QSqlQuery q;
    QVERIFY(q.exec("CREATE TABLE users(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"));
    QVERIFY(q.exec("INSERT INTO users(name) VALUES('alice');"));
    db.close();

    QSqlDatabase remoteSetup = QSqlDatabase::addDatabase("QSQLITE", "setup");
    remoteSetup.setDatabaseName(remotePath);
    QVERIFY(remoteSetup.open());
    QSqlQuery qr(remoteSetup);
    QVERIFY(qr.exec("CREATE TABLE users(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"));
    remoteSetup.close();
    QSqlDatabase::removeDatabase("setup");

    QVERIFY(db.open());
    QVERIFY(db.synchronize());
    db.close();

    QSqlDatabase verify = QSqlDatabase::addDatabase("QSQLITE", "verify");
    verify.setDatabaseName(remotePath);
    QVERIFY(verify.open());
    QSqlQuery qv(verify);
    QVERIFY(qv.exec("SELECT name FROM users"));
    QVERIFY(qv.next());
    QCOMPARE(qv.value(0).toString(), QString("alice"));
    verify.close();
    QSqlDatabase::removeDatabase("verify");

    qunsetenv("NIES_DB_OFFLINE");
    qunsetenv("NIES_DB_OFFLINE_PATH");
    qunsetenv("NIES_DB_DRIVER");
    qunsetenv("NIES_DB_NAME");
}

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    QApplication app(argc, argv);
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
    InventoryManagerTest invTest;
    status |= QTest::qExec(&invTest, argc, argv);
    OfflineSyncTest offlineTest;
    status |= QTest::qExec(&offlineTest, argc, argv);
    LoginDialogTest loginTest;
    status |= QTest::qExec(&loginTest, argc, argv);
    ProductWindowTest productWindowTest;
    status |= QTest::qExec(&productWindowTest, argc, argv);
    POSWindowTest posWindowTest;
    status |= QTest::qExec(&posWindowTest, argc, argv);
    SalesReportWindowTest salesReportTest;
    status |= QTest::qExec(&salesReportTest, argc, argv);
    MainWindowTest mainWinTest;
    status |= QTest::qExec(&mainWinTest, argc, argv);
    return status;
}

#include "database_test.moc"

