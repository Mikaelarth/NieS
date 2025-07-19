#include <QtTest>
#include <QApplication>

#include "payments/PaymentProcessor.h"
#include "payment_processor_test.h"

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

