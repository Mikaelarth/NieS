#ifndef INVOICE_PRINTER_TEST_H
#define INVOICE_PRINTER_TEST_H

#include <QObject>

class InvoicePrinterTest : public QObject
{
    Q_OBJECT
private slots:
    void printCreatesFile();
};

#endif // INVOICE_PRINTER_TEST_H
