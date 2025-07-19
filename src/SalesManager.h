#ifndef SALESMANAGER_H
#define SALESMANAGER_H

#include <QObject>
#include <QList>
#include <QVariantMap>
#include "InventoryManager.h"

class SalesManager : public QObject
{
    Q_OBJECT
public:
    explicit SalesManager(QObject *parent = nullptr);

    bool recordSale(int productId, int quantity);
    QList<QVariantMap> salesReport();
    QVariantMap financialReport();

    QString lastError() const;

private:
    QString m_lastError;
    InventoryManager m_inventory;
};

#endif // SALESMANAGER_H
