#ifndef INVENTORY_WINDOW_TEST_H
#define INVENTORY_WINDOW_TEST_H

#include <QObject>

class InventoryWindowTest : public QObject
{
    Q_OBJECT
private slots:
    void scanAdjustsQuantity();
};

#endif // INVENTORY_WINDOW_TEST_H
