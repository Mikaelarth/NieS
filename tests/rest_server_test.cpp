#include <QtTest>
#include <QApplication>
#include <QTcpSocket>
#include "api/RestServer.h"
#include "rest_server_test.h"

void RestServerTest::startAndRespond()
{
    RestServer server;
    QVERIFY(server.start(12345));

    QTcpSocket sock;
    sock.connectToHost("127.0.0.1", 12345);
    QVERIFY(sock.waitForConnected());

    QByteArray req = "GET /bad HTTP/1.1\r\nHost: localhost\r\n\r\n";
    QVERIFY(sock.write(req) == req.size());
    QVERIFY(sock.waitForBytesWritten());
    QVERIFY(sock.waitForReadyRead());
    QByteArray resp = sock.readAll();
    QVERIFY(resp.startsWith("HTTP/1.1 404"));
}

#include "rest_server_test.moc"
