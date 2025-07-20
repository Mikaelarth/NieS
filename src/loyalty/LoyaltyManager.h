#ifndef LOYALTYMANAGER_H
#define LOYALTYMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVariantMap>

class LoyaltyManager : public QObject
{
    Q_OBJECT
public:
    explicit LoyaltyManager(QObject *parent = nullptr);

    bool createAccount(const QString &phone);
    bool changePoints(const QString &phone, int delta, const QString &reason = QString());
    int points(const QString &phone) const;
    QList<QVariantMap> listAccounts();

    QString lastError() const;

private:
    int accountId(const QString &phone) const;
    QString m_lastError;
};

#endif // LOYALTYMANAGER_H
