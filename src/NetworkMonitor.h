#ifndef NETWORKMONITOR_H
#define NETWORKMONITOR_H

#include <QObject>
#include <QNetworkConfigurationManager>

class NetworkMonitor : public QObject
{
    Q_OBJECT
public:
    explicit NetworkMonitor(QObject *parent = nullptr);
    bool isOnline() const { return m_isOnline; }

signals:
    void connectivityChanged(bool online);

private slots:
    void handleOnlineStateChanged(bool isOnline);

private:
    QNetworkConfigurationManager m_manager;
    bool m_isOnline;
};

#endif // NETWORKMONITOR_H
