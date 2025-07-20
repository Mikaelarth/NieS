#ifndef LOYALTY_MANAGER_TEST_H
#define LOYALTY_MANAGER_TEST_H

#include <QObject>

class LoyaltyManagerTest : public QObject
{
    Q_OBJECT
private slots:
    void addPointsCreatesAccount();
};

#endif // LOYALTY_MANAGER_TEST_H
