#include "payments/PaymentProcessor.h"
#include "DatabaseManager.h"

#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

PaymentProcessor::PaymentProcessor(DatabaseManager *db, QNetworkAccessManager *manager, QObject *parent)
    : QObject(parent),
      m_manager(manager ? manager : new QNetworkAccessManager(this))
{
    if (db) {
        m_apiKey = db->paymentApiKey();
        m_endpoint = QUrl(db->paymentEndpoint());
    }
}

void PaymentProcessor::setApiCredentials(const QString &key, const QUrl &endpoint)
{
    m_apiKey = key;
    m_endpoint = endpoint;
}

bool PaymentProcessor::processCard(double amount)
{
    return sendRequest("card", amount);
}

bool PaymentProcessor::processMobileMoney(double amount)
{
    return sendRequest("mobile", amount);
}

bool PaymentProcessor::processQrCode(double amount)
{
    return sendRequest("qrcode", amount);
}

QString PaymentProcessor::lastError() const
{
    return m_lastError;
}

bool PaymentProcessor::sendRequest(const QString &method, double amount)
{
    if (amount <= 0) {
        m_lastError = tr("Invalid amount");
        return false;
    }

    if (m_endpoint.isEmpty()) {
        m_lastError = tr("Payment endpoint not configured");
        return false;
    }
    if (m_apiKey.isEmpty()) {
        m_lastError = tr("Missing API key");
        return false;
    }

    QNetworkRequest req(m_endpoint);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject obj{{"method", method}, {"amount", amount}, {"api_key", m_apiKey}};
    QNetworkReply *reply = m_manager->post(req, QJsonDocument(obj).toJson());

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        m_lastError = reply->errorString();
        reply->deleteLater();
        return false;
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        m_lastError = tr("Invalid response");
        return false;
    }

    QJsonObject resp = doc.object();
    bool success = resp.value("success").toBool(true);
    if (!success) {
        m_lastError = resp.value("error").toString(tr("Payment failed"));
        return false;
    }

    return true;
}

