#ifndef NETWORKMONITOR_H
#define NETWORKMONITOR_H

#include <QObject>
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
#  include <QNetworkInformation>
#else
#  include <QNetworkConfigurationManager>
#endif

class NetworkMonitor : public QObject
{
    Q_OBJECT
public:
    explicit NetworkMonitor(QObject *parent = nullptr);

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
    using Reachability = QNetworkInformation::Reachability;
#else
    enum class Reachability { Unknown, Disconnected, Local, Site, Online };
#endif

    Reachability reachability() const;
    bool isOnline() const;

    void startMonitoring();

signals:
    void networkStatusChanged(Reachability reachability);

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
private slots:
    void handleOnlineStateChanged(bool isOnline);
#endif

private:
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
    QNetworkInformation *m_info;
    Reachability m_reachability;
#else
    QNetworkConfigurationManager m_manager;
    bool m_isOnline;
    Reachability m_reachability;
#endif
};

#endif // NETWORKMONITOR_H
