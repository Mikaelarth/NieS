#ifndef RETURNMANAGER_H
#define RETURNMANAGER_H

#include <QObject>
#include <QString>

class ReturnManager : public QObject
{
    Q_OBJECT
public:
    explicit ReturnManager(QObject *parent = nullptr);

    bool recordReturn(int saleId, int quantity);
    QString lastError() const;

private:
    QString m_lastError;
};

#endif // RETURNMANAGER_H
