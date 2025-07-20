#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "RestClient.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    RestClient client;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("restClient", &client);
    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
