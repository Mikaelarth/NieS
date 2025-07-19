#include "payments/PaymentProcessor.h"
#include <QDebug>

PaymentProcessor::PaymentProcessor(QObject *parent)
    : QObject(parent)
{
}

bool PaymentProcessor::processCard(double amount)
{
    if (amount <= 0) {
        m_lastError = tr("Invalid amount");
        return false;
    }

    // Simulation of a real card payment. Replace with actual gateway call.
    qDebug() << "Simulated card payment:" << amount;
    return true;
}

bool PaymentProcessor::processMobileMoney(double amount)
{
    if (amount <= 0) {
        m_lastError = tr("Invalid amount");
        return false;
    }

    qDebug() << "Simulated mobile money payment:" << amount;
    return true;
}

bool PaymentProcessor::processQrCode(double amount)
{
    if (amount <= 0) {
        m_lastError = tr("Invalid amount");
        return false;
    }

    qDebug() << "Simulated QR code payment:" << amount;
    return true;
}

QString PaymentProcessor::lastError() const
{
    return m_lastError;
}

