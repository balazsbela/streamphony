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
        const QString hash = QString::fromUtf8(m_dhtManager->hash(uniqueData));
        debugConnectionManager() << "Launching ip query for:" << jid << ":" << m_xmppManager->fullName(jid);
        nodeIdMap[hash] = jid;
        m_dhtManager->findNode(hash);
    };

    connect(m_dhtManager.data(), &DhtManager::peerIpFound, [&](const QString &id, const QHostAddress &ip, const quint16 port) {
        debugConnectionManager() << "Received ip for:" << nodeIdMap[id];
        m_nodeHash[nodeIdMap[id]] = QSharedPointer<Node>(new Node(id, ip, port));
    });

    for (const QString &jid : m_xmppManager->allAvailableJids()) {
        const QByteArray uniqueData = m_xmppManager->userUniqueId(jid);
        if (uniqueData.size() == 0) {
            QSharedPointer<QTimer> timer;
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
