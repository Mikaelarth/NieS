#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#include <QObject>
#include <QUrl>

class QNetworkAccessManager;

class RestClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
public:
    explicit RestClient(const QUrl &baseUrl = QUrl("http://localhost:8080"), QObject *parent = nullptr);

    QUrl baseUrl() const { return m_baseUrl; }
    void setBaseUrl(const QUrl &url);

    Q_INVOKABLE void fetchProducts();
    Q_INVOKABLE void postSale(int productId, int quantity);

signals:
    void baseUrlChanged();
    void productsReceived(const QVariantList &products);
    void saleFinished(bool success, const QString &error);

private:
    QNetworkAccessManager *m_mgr;
    QUrl m_baseUrl;
};

#endif // RESTCLIENT_H
