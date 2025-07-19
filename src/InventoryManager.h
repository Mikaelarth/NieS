#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include <QObject>
#include <QString>

class UserSession;

class InventoryManager : public QObject
{
    Q_OBJECT
public:
    explicit InventoryManager(UserSession *session = nullptr, QObject *parent = nullptr);

    bool addStock(int productId, int quantity);
    bool removeStock(int productId, int quantity);
    int currentStock(int productId) const;
    bool isCritical(int productId, int threshold) const;

    QString lastError() const;

private:
    bool updateStock(int productId, int delta);
    QString m_lastError;
    UserSession *m_session = nullptr;
};

#endif // INVENTORYMANAGER_H
