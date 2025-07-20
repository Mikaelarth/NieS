#include <QCoreApplication>
#include <QDebug>
#include "RestServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    RestServer server;
    if (!server.start(8080)) {
        qCritical("Failed to start REST server");
        return 1;
    }

    return app.exec();
}
