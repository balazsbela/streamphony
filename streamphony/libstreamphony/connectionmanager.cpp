#include "connectionmanager.h"

ConnectionManager::ConnectionManager(XmppManager *xmppManager, QObject *parent) :
    QObject(parent)
{
    Q_ASSERT(xmppManager);
    m_xmppManager.reset(xmppManager);
}

ConnectionManager::~ConnectionManager()
{
}

