#include <QCoreApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <QProcessEnvironment>
#include "RestServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption portOpt(QStringList{"p", "port"},
                               "Port to listen on", "port");
    parser.addOption(portOpt);
    parser.process(app);

    int port = 8080;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if (env.contains("NIES_REST_PORT"))
        port = env.value("NIES_REST_PORT").toInt();
    if (parser.isSet(portOpt))
        port = parser.value(portOpt).toInt();

    RestServer server;
    if (!server.start(static_cast<quint16>(port))) {
        qCritical("Failed to start REST server");
        return 1;
    }

    return app.exec();
}
