#include "NetworkMonitor.h"

NetworkMonitor::NetworkMonitor(QObject *parent)
    : QObject(parent),
      m_isOnline(m_manager.isOnline())
{
    connect(&m_manager, &QNetworkConfigurationManager::onlineStateChanged,
            this, &NetworkMonitor::handleOnlineStateChanged);
}

void NetworkMonitor::handleOnlineStateChanged(bool isOnline)
{
    if (m_isOnline == isOnline)
        return;
    m_isOnline = isOnline;
    emit connectivityChanged(m_isOnline);
}
