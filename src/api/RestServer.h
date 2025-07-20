#ifndef RESTSERVER_H
#define RESTSERVER_H

#include <QObject>

class QTcpServer;
class QTcpSocket;
class DatabaseManager;
class ProductManager;
class SalesManager;

class RestServer : public QObject
{
    Q_OBJECT
public:
    explicit RestServer(QObject *parent = nullptr);
    bool start(quint16 port = 8080);

private:
    void handleConnection();

    QTcpServer *m_server;
    DatabaseManager *m_db;
    ProductManager *m_products;
    SalesManager *m_sales;
};

#endif // RESTSERVER_H
