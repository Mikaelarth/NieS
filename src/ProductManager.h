#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H

#include <QObject>
#include <QList>
#include <QVariantMap>

class UserSession;

class ProductManager : public QObject
{
    Q_OBJECT
public:
    explicit ProductManager(UserSession *session = nullptr, QObject *parent = nullptr);

    bool addProduct(const QString &name, double price, double discount = 0.0);
    bool updateProduct(int id, const QString &name, double price, double discount);
    bool deleteProduct(int id);
    QList<QVariantMap> listProducts();

    QString lastError() const;

private:
    QString m_lastError;
    UserSession *m_session = nullptr;
};

#endif // PRODUCTMANAGER_H
