#include "barcode/BarcodeScanner.h"

BarcodeScanner::BarcodeScanner(QObject *parent)
    : QObject(parent)
{
}

void BarcodeScanner::scan(const QString &code)
{
    emit codeScanned(code);
}

