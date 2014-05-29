#include "connectionmanager.h"
#include "dht/dhtmanager.h"
#include "daemon/node.h"

#include <QCryptographicHash>
#include <QTimer>
#include <QFile>

#define DEBUG_CONNECTION_MANAGER

#ifdef DEBUG_CONNECTION_MANAGER
#define debugConnectionManager qDebug
#else
#define debugConnectionManager QT_NO_QDEBUG_MACRO
#endif

static const QString NODE_CACHE_FILE = QStringLiteral("nodeCache.txt");

ConnectionManager::ConnectionManager(DhtManager *dhtManager, XmppManager *xmppManager, QObject *parent) :
    QObject(parent)
{
    Q_ASSERT(xmppManager);
    Q_ASSERT(dhtManager);

    m_xmppManager.reset(xmppManager);
    m_dhtManager.reset(dhtManager);
    loadNodes();
}

ConnectionManager::~ConnectionManager()
{    
}

void ConnectionManager::populateNodeHash()
{
    auto launchQuery = [&] (const QByteArray &uniqueData, const QString &jid) {
        Q_ASSERT(!m_xmppManager->fullName(jid).isEmpty());
        QString hash = uniqueData.toHex();

        if (nodeIdMap[hash] != nullptr && nodeIdMap[hash]->jid() == jid) {
            if (nodeIdMap[hash]->queryRunning()) {
                return;
            }
        }
        debugConnectionManager() << "Launching ip query for:" << jid << ":" << m_xmppManager->fullName(jid) << hash;
        nodeIdMap[hash] = QSharedPointer<NodeStatus>(new NodeStatus(jid, true));
        m_dhtManager->findNodeByHash(uniqueData);
    };

    // Handle response
    connect(m_dhtManager.data(), &DhtManager::peerIpFound, [&](const QString &id, const QHostAddress &ip, const quint16 port) {
        Q_ASSERT(nodeIdMap[id]!=nullptr);
        debugConnectionManager() << "Received ip for:" << nodeIdMap[id]->jid();
        m_nodeHash[nodeIdMap[id]->jid()] = QSharedPointer<Node>(new Node(id, ip, port));
        nodeIdMap[id] = QSharedPointer<NodeStatus>(new NodeStatus(nodeIdMap[id]->jid(), false));
        saveNodes();
    });

    // Launch queries
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

void ConnectionManager::saveNodes() {
    QFile cacheFile(NODE_CACHE_FILE);
    if (!cacheFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not create node cache file!";
    }

    QTextStream out(&cacheFile);

    for (const QString &key : m_nodeHash.keys()) {
        out << key << " " << m_nodeHash[key]->host().toString() << " " << m_nodeHash[key]->port() << endl;
    }

    cacheFile.close();
}

void ConnectionManager::loadNodes() {
    QFile cacheFile(NODE_CACHE_FILE);

    if (!cacheFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Couldn't open node cache";
        return;
    }

    QTextStream in(&cacheFile);

    while (!in.atEnd()) {
        const QString &line = in.readLine();
        const QStringList &tokens = line.split(" ");
        Q_ASSERT(tokens.size() == 3);
        const QString &id = tokens.first();
        const QHostAddress &host = QHostAddress(tokens[1]);

        bool ok = false;
        const quint32 port = tokens[3].toUInt(&ok);
        Q_ASSERT(ok);

        m_nodeHash[tokens.first()] = QSharedPointer<Node>(new Node(id, host, port));
    }

    cacheFile.close();
}
