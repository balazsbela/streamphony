#include "connectionmanager.h"
#include "dht/dhtmanager.h"
#include "daemon/node.h"

#include <QCryptographicHash>

#define DEBUG_CONNECTION_MANAGER

#ifdef DEBUG_CONNECTION_MANAGER
#define debugConnectionManager qDebug
#else
#define debugConnectionManager QT_NO_QDEBUG_MACRO
#endif

ConnectionManager::ConnectionManager(DhtManager *dhtManager, XmppManager *xmppManager, QObject *parent) :
    QObject(parent)
{
    Q_ASSERT(xmppManager);
    Q_ASSERT(dhtManager);

    m_xmppManager.reset(xmppManager);
    m_dhtManager.reset(dhtManager); 
}

ConnectionManager::~ConnectionManager()
{
}

void ConnectionManager::populateNodeHash()
{
    connect(m_dhtManager.data(), &DhtManager::peerIpFound, [&](const QString &id, const QHostAddress &ip, const quint16 port) {
        debugConnectionManager() << "Received ip for:" << nodeIdMap[id];
        m_nodeHash[nodeIdMap[id]] = QSharedPointer<Node>(new Node(id, ip, port));
    });

    for (const QString &jid : m_xmppManager->allAvailableJids()) {
        debugConnectionManager() << "Launching ip query for:" << jid;

        const QByteArray uniqueData = m_xmppManager->userUniqueId(jid);
        const QString hash = QString::fromUtf8(m_dhtManager->hash(uniqueData));

        nodeIdMap[hash] = jid;
        m_dhtManager->findNode(hash);
    }
}
