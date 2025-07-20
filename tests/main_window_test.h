#ifndef MAIN_WINDOW_TEST_H
#define MAIN_WINDOW_TEST_H

#include <QObject>

class MainWindowTest : public QObject
{
    Q_OBJECT
private slots:
    void adminFullAccess();
    void sellerLimitedAccess();
    void viewerNoAccess();
    void dashboardActionOpens();
};

#endif // MAIN_WINDOW_TEST_H
