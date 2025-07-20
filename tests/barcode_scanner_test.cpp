#include <QtTest>
#include <QSignalSpy>
#include "barcode/BarcodeScanner.h"
#include "barcode_scanner_test.h"

void BarcodeScannerTest::emitsSignal()
{
    BarcodeScanner scanner;
    QSignalSpy spy(&scanner, &BarcodeScanner::codeScanned);
    scanner.scan("12345");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), QString("12345"));
}

