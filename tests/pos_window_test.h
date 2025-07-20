#ifndef POS_WINDOW_TEST_H
#define POS_WINDOW_TEST_H

#include <QObject>

class POSWindowTest : public QObject
{
    Q_OBJECT
private slots:
    void sellItemUpdatesInventory();
    void printInvoiceCreatesFile();
};

#endif // POS_WINDOW_TEST_H
