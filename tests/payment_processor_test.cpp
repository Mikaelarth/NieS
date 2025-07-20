#include <QtTest>
#include <QApplication>

#include "payments/PaymentProcessor.h"
#include "payment_processor_test.h"

class MockProcessor : public PaymentProcessor
{
public:
    QString method;
    double amount = 0;
    bool result = true;
protected:
    bool sendRequest(const QString &m, double a) override
    {
        method = m;
        amount = a;
        return result;
    }
};

void PaymentProcessorTest::invalidAmountsFail()
{
    PaymentProcessor p;
    QVERIFY(!p.processCard(0));
    QCOMPARE(p.lastError(), QString("Invalid amount"));
    QVERIFY(!p.processMobileMoney(-5));
    QCOMPARE(p.lastError(), QString("Invalid amount"));
    QVERIFY(!p.processQrCode(0));
    QCOMPARE(p.lastError(), QString("Invalid amount"));
}

void PaymentProcessorTest::validAmountsSucceed()
{
    PaymentProcessor p;
    QVERIFY(p.processCard(10));
    QVERIFY(p.processMobileMoney(5));
    QVERIFY(p.processQrCode(1));
}

void PaymentProcessorTest::gatewayCalled()
{
    MockProcessor p;
    QVERIFY(p.processCard(7));
    QCOMPARE(p.method, QString("card"));
    QCOMPARE(p.amount, 7.0);
}

