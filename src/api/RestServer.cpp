#include "RestServer.h"
#include "DatabaseManager.h"
#include "ProductManager.h"
#include "SalesManager.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QHostAddress>
#include <QVariantMap>
#include <QDebug>

RestServer::RestServer(QObject *parent)
    : QObject(parent), m_server(new QTcpServer(this)),
      m_db(new DatabaseManager(QString(), this)),
      m_products(new ProductManager(nullptr, this)),
      m_sales(new SalesManager(nullptr, this))
{
    if (!m_db->open())
        qWarning() << "Database open failed:" << m_db->lastError();
}

static QJsonArray variantListToJson(const QList<QVariantMap> &list)
{
    QJsonArray arr;
    for (const QVariantMap &m : list)
        arr.append(QJsonObject::fromVariantMap(m));
    return arr;
}

bool RestServer::start(quint16 port)
{
    connect(m_server, &QTcpServer::newConnection, this, &RestServer::handleConnection);
    if (!m_server->listen(QHostAddress::Any, port))
        return false;
    qInfo() << "REST server listening on port" << port;
    return true;
}

void RestServer::handleConnection()
{
    QTcpSocket *socket = m_server->nextPendingConnection();
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    connect(socket, &QTcpSocket::readyRead, this, [=]() {
        QByteArray request = socket->readAll();
        QList<QByteArray> lines = request.split('\n');
        if (lines.isEmpty()) {
            socket->disconnectFromHost();
            return;
        }
        QByteArray requestLine = lines.first().trimmed();
        QList<QByteArray> parts = requestLine.split(' ');
        QByteArray method = parts.value(0);
        QByteArray path = parts.value(1);
        int bodyIndex = request.indexOf("\r\n\r\n");
        QByteArray body = bodyIndex != -1 ? request.mid(bodyIndex + 4) : QByteArray();

        QByteArray response;
        if (method == "GET" && path == "/products") {
            QJsonArray arr = variantListToJson(m_products->listProducts());
            QByteArray json = QJsonDocument(arr).toJson();
            response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: "
                       + QByteArray::number(json.size()) + "\r\n\r\n" + json;
        } else if (method == "POST" && path == "/products") {
            QJsonObject obj = QJsonDocument::fromJson(body).object();
            QString name = obj.value("name").toString();
            double price = obj.value("price").toDouble();
            double discount = obj.value("discount").toDouble();
            bool ok = m_products->addProduct(name, price, discount);
            response = QByteArray("HTTP/1.1 ") + (ok ? "201 Created" : "500 Internal Server Error")
                    + "\r\n\r\n";
        } else if (method == "GET" && path == "/sales") {
            QJsonArray arr = variantListToJson(m_sales->salesReport());
            QByteArray json = QJsonDocument(arr).toJson();
            response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: "
                       + QByteArray::number(json.size()) + "\r\n\r\n" + json;
        } else if (method == "POST" && path == "/sales") {
            QJsonObject obj = QJsonDocument::fromJson(body).object();
            int pid = obj.value("product_id").toInt();
            int qty = obj.value("quantity").toInt();
            bool ok = m_sales->recordSale(pid, qty);
            response = QByteArray("HTTP/1.1 ") + (ok ? "201 Created" : "500 Internal Server Error")
                    + "\r\n\r\n";
        } else {
            response = "HTTP/1.1 404 Not Found\r\n\r\n";
        }
        socket->write(response);
        socket->disconnectFromHost();
    });
}
