#include "NetworkMonitor.h"

NetworkMonitor::NetworkMonitor(QObject *parent)
    : QObject(parent)
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
    , m_info(QNetworkInformation::instance())
    , m_reachability(m_info->reachability())
#else
    , m_isOnline(m_manager.isOnline())
    , m_reachability(m_isOnline ? Reachability::Online : Reachability::Disconnected)
#endif
{
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)

void NetworkMonitor::startMonitoring()
{
    connect(m_info, &QNetworkInformation::reachabilityChanged, this, [this](QNetworkInformation::Reachability r){
        if (m_reachability == r)
            return;
        m_reachability = r;
        emit networkStatusChanged(r);
    });
}

NetworkMonitor::Reachability NetworkMonitor::reachability() const
{
    return m_reachability;
}

bool NetworkMonitor::isOnline() const
{
    return m_reachability != QNetworkInformation::Reachability::Disconnected;
}

#else

void NetworkMonitor::startMonitoring()
{
    connect(&m_manager, &QNetworkConfigurationManager::onlineStateChanged,
            this, &NetworkMonitor::handleOnlineStateChanged);
}

void NetworkMonitor::handleOnlineStateChanged(bool isOnline)
{
    if (m_isOnline == isOnline)
        return;
    m_isOnline = isOnline;
    m_reachability = m_isOnline ? Reachability::Online : Reachability::Disconnected;
    emit networkStatusChanged(m_reachability);
}

NetworkMonitor::Reachability NetworkMonitor::reachability() const
{
    return m_reachability;
}

bool NetworkMonitor::isOnline() const
{
    return m_isOnline;
}

#endif
