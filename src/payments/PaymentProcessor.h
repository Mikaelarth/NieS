#ifndef PAYMENTPROCESSOR_H
#define PAYMENTPROCESSOR_H

#include <QObject>
#include <QString>

// Basic payment handler used by the POS window. The current implementation
// only simulates successful transactions. Integrate with a real payment
// gateway to accept actual payments.
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
