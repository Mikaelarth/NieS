#ifndef SALESMANAGER_H
#define SALESMANAGER_H

#include <QObject>
#include <QList>
#include <QVariantMap>
#include "InventoryManager.h"

class UserSession;

class SalesManager : public QObject
{
    Q_OBJECT
public:
    explicit SalesManager(UserSession *session = nullptr, QObject *parent = nullptr);

    bool recordSale(int productId, int quantity);
    QList<QVariantMap> salesReport();
    QVariantMap financialReport();

    QString lastError() const;

signals:
    void saleRecorded(int productId, int quantity);

private:
    QString m_lastError;
    UserSession *m_session = nullptr;
    InventoryManager m_inventory;
};

#endif // SALESMANAGER_H
