#ifndef BARCODESCANNER_H
#define BARCODESCANNER_H

#include <QObject>
#include <QString>

class BarcodeScanner : public QObject
{
    Q_OBJECT
public:
    explicit BarcodeScanner(QObject *parent = nullptr);

public slots:
    void scan(const QString &code);

signals:
    void codeScanned(const QString &code);
};

#endif // BARCODESCANNER_H
