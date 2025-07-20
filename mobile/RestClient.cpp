#include "RestClient.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariantList>

RestClient::RestClient(const QUrl &baseUrl, QObject *parent)
    : QObject(parent), m_mgr(new QNetworkAccessManager(this)), m_baseUrl(baseUrl)
{
}

void RestClient::setBaseUrl(const QUrl &url)
{
    if (m_baseUrl == url)
        return;
    m_baseUrl = url;
    emit baseUrlChanged();
}

void RestClient::fetchProducts()
{
    QNetworkRequest req(m_baseUrl.resolved(QUrl("/products")));
    QNetworkReply *reply = m_mgr->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QByteArray data = reply->readAll();
        reply->deleteLater();
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError || !doc.isArray()) {
            emit productsReceived({});
            return;
        }
        QVariantList list = doc.array().toVariantList();
        emit productsReceived(list);
    });
}

void RestClient::postSale(int productId, int quantity)
{
    QNetworkRequest req(m_baseUrl.resolved(QUrl("/sales")));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject obj{{"product_id", productId}, {"quantity", quantity}};
    QNetworkReply *reply = m_mgr->post(req, QJsonDocument(obj).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        bool ok = reply->error() == QNetworkReply::NoError;
        QString err = ok ? QString() : reply->errorString();
        reply->deleteLater();
        emit saleFinished(ok, err);
    });
}
