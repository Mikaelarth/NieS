#ifndef PAYMENTPROCESSOR_H
#define PAYMENTPROCESSOR_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>

class DatabaseManager;

// Basic payment handler used by the POS window. It now performs real
// HTTP requests to the configured payment gateway. If no endpoint is set
// the transaction fails with an error.
class PaymentProcessor : public QObject
{
    Q_OBJECT
public:
    explicit PaymentProcessor(DatabaseManager *db = nullptr,
                              QNetworkAccessManager *manager = nullptr,
                              QObject *parent = nullptr);
    void setApiCredentials(const QString &key, const QUrl &endpoint);

    bool processCard(double amount);
    bool processMobileMoney(double amount);
    bool processQrCode(double amount);

    QString lastError() const;

protected:
    virtual bool sendRequest(const QString &method, double amount);

private:
    QString m_lastError;
    QNetworkAccessManager *m_manager;
    QString m_apiKey;
    QUrl m_endpoint;
};

#endif // PAYMENTPROCESSOR_H
