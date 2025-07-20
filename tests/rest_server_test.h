#ifndef REST_SERVER_TEST_H
#define REST_SERVER_TEST_H

#include <QObject>

class RestServerTest : public QObject
{
    Q_OBJECT
private slots:
    void startAndRespond();
};

#endif // REST_SERVER_TEST_H
