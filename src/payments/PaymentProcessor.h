#ifndef PAYMENTPROCESSOR_H
#define PAYMENTPROCESSOR_H

#include <QObject>
#include <QString>

class PaymentProcessor : public QObject
{
    Q_OBJECT
public:
    explicit PaymentProcessor(QObject *parent = nullptr);

    bool processCard(double amount);
    bool processMobileMoney(double amount);
    bool processQrCode(double amount);

    QString lastError() const;

private:
    QString m_lastError;
};

#endif // PAYMENTPROCESSOR_H
