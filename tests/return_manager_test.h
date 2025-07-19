#ifndef RETURN_MANAGER_TEST_H
#define RETURN_MANAGER_TEST_H

#include <QObject>

class ReturnManagerTest : public QObject
{
    Q_OBJECT
private slots:
    void recordReturnUpdatesInventory();
};

#endif // RETURN_MANAGER_TEST_H
