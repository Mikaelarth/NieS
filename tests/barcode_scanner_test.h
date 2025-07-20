#ifndef BARCODE_SCANNER_TEST_H
#define BARCODE_SCANNER_TEST_H

#include <QObject>

class BarcodeScannerTest : public QObject
{
    Q_OBJECT
private slots:
    void emitsSignal();
};

#endif // BARCODE_SCANNER_TEST_H
