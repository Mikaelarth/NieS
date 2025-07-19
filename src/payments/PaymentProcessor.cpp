#include "payments/PaymentProcessor.h"

PaymentProcessor::PaymentProcessor(QObject *parent)
    : QObject(parent)
{
}

bool PaymentProcessor::processCard(double amount)
{
    Q_UNUSED(amount);
    // Here an actual card payment API would be called.
    return true;
}

bool PaymentProcessor::processMobileMoney(double amount)
{
    Q_UNUSED(amount);
    return true;
}

bool PaymentProcessor::processQrCode(double amount)
{
    Q_UNUSED(amount);
    return true;
}

QString PaymentProcessor::lastError() const
{
    return m_lastError;
}

