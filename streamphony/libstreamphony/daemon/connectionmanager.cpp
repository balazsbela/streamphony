#include "connectionmanager.h"
#include "dht/dhtmanager.h"
#include "daemon/node.h"

#include <QCryptographicHash>
#include <QTimer>

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
    auto launchQuery = [&] (const QByteArray &uniqueData, const QString &jid) {
        Q_ASSERT(!m_xmppManager->fullName(jid).isEmpty());
        //if (m_xmppManager->fullName(jid).startsWith("Sebes")) {
            QString hash = uniqueData.toHex();

            if (nodeIdMap[hash] != nullptr && nodeIdMap[hash]->jid() == jid) {
                if (nodeIdMap[hash]->queryRunning()) {
                    debugConnectionManager() << "Query already running, not launching again!";
                    return;
                }
            }
            debugConnectionManager() << "Launching ip query for:" << jid << ":" << m_xmppManager->fullName(jid) << hash;
            nodeIdMap[hash] = QSharedPointer<NodeStatus>(new NodeStatus(jid, true));
            m_dhtManager->findNodeByHash(uniqueData);
        //}
    };

    connect(m_dhtManager.data(), &DhtManager::peerIpFound, [&](const QString &id, const QHostAddress &ip, const quint16 port) {
        Q_ASSERT(nodeIdMap[id]!=nullptr);
        debugConnectionManager() << "Received ip for:" << nodeIdMap[id]->jid();
        m_nodeHash[nodeIdMap[id]->jid()] = QSharedPointer<Node>(new Node(id, ip, port));
        nodeIdMap[id] = QSharedPointer<NodeStatus>(new NodeStatus(nodeIdMap[id]->jid(), false));
    });

    for (const QString &jid : m_xmppManager->allAvailableJids()) {
        const QByteArray uniqueData = m_xmppManager->userUniqueId(jid);
        if (uniqueData.size() == 0) {
            QSharedPointer<QTimer> timer = QSharedPointer<QTimer>(new QTimer());
            timer->setInterval(5000);
            connect(timer.data(), &QTimer::timeout, [&]() {
                const QByteArray &data = m_xmppManager->userUniqueId(jid);
                if (data.size() != 0) {
                    launchQuery(data, jid);
                    timer->stop();
                }
            });
            timer->start();
        } else {
            launchQuery(uniqueData, jid);
        }
    }
}
