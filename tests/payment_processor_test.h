#ifndef PAYMENT_PROCESSOR_TEST_H
#define PAYMENT_PROCESSOR_TEST_H

#include <QObject>

class PaymentProcessorTest : public QObject
{
    Q_OBJECT
private slots:
    void invalidAmountsFail();
    void validAmountsSucceed();
};

#endif // PAYMENT_PROCESSOR_TEST_H
