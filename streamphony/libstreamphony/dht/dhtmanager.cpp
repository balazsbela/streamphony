#include "dhtmanager.h"
#include "dhtdebug.h"
#include "dhtcallbacks.h"
#include "singleshottimer.h"
#include "dhtconversions.h"

#include "bitdht/bdiface.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <memory>
#include <sstream>

#include <QCryptographicHash>
#include <QTcpSocket>
#include <QFile>

static const std::string SECRET = "StreamphonySecret";
static const int PUSH_INTERVAL = 60000;
static const QString DHT_FILE = "hashSet.txt";
static const int MAX_QUERY_PER_MINUTE = 5;

using namespace DhtConversions;

DhtManager::DhtManager(QObject *parent) : QObject(parent)
{    
}

void DhtManager::start(bdNodeId *ownId, uint16_t port, const QString &appId, const QString &bootstrapFile)
{
    m_ownId = ownId;

    qDebug() << QString::fromStdString(m_ownIp);

    debugDht() << "Using id:";
    bdStdPrintNodeId(std::cerr, ownId);
    debugDht() << endl;
    debugDht() << endl << "Using bootstrap file:" << bootstrapFile;

    /* standard dht behaviour */
    bdDhtFunctions *stdFns = new bdStdDht();

    m_bdSpace.reset(new bdSpace(ownId, stdFns));

    debugDht() << "Creating UDP DHT:";

    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = 0;
    local.sin_port = htons(port);

    m_stack.reset(new UdpStack(local));
    m_udpBitDht.reset(new UdpBitDht(m_stack.data(), ownId, appId.toStdString(), bootstrapFile.toStdString(), stdFns));
    m_stack->addReceiver(m_udpBitDht.data());

    // Setup callback interface
    DhtCallbacks *callbacks = new DhtCallbacks(this);
    m_udpBitDht->addCallback(callbacks);

    // Start bitDHT thread
    m_udpBitDht->start();

    // Start DHT
    m_udpBitDht->startDht();

    utils::singleShotTimer(15000, [&]() {
        qDebug() << m_udpBitDht->statsNetworkSize();
    }, nullptr);

    setupQueryTimer();

    QFile file(DHT_FILE);
    file.remove();
}


DhtManager::~DhtManager()
{
    m_udpBitDht->stopDht();
}

void DhtManager::enable(bool on)
{
    if (isEnabled() == on)
        return;

    if (on)
        m_udpBitDht->startDht();
    else
        m_udpBitDht->stopDht();
}

void DhtManager::shutdown()
{
    m_udpBitDht->stopDht(); // blocking call
}

void DhtManager::restart()
{
    m_udpBitDht->stopDht();
    m_udpBitDht->startDht();
}

bool DhtManager::isEnabled()
{
    return m_udpBitDht->stateDht() != 0;
}

bool DhtManager::isActive()
{
    return m_udpBitDht->stateDht() >= BITDHT_MGR_STATE_ACTIVE;
}

bool DhtManager::findNode(bdNodeId *peerId)
{
    std::cerr << "BitDhtHandler::FindNode(";
    bdStdPrintNodeId(std::cerr, peerId);
    std::cerr << ")" << std::endl;

    m_udpBitDht->addFindNode(peerId, BITDHT_QFLAGS_UPDATES);

    return true;
}

bool DhtManager::dropNode(bdNodeId *peerId)
{
    std::cerr << "BitDhtHandler::DropNode(";
    bdStdPrintNodeId(std::cerr, peerId);
    std::cerr << ")" << std::endl;
    std::cerr << std::endl;

    m_udpBitDht->removeFindNode(peerId);
    return true ;
}

void DhtManager::foundPeer(const bdId *id, uint32_t status)
{      

    if (status != BITDHT_QUERY_FOUND_CLOSEST || status != BITDHT_QUERY_SUCCESS) {

        bdStdPrintNodeId(std::cout, &id->id);
        qDebug() << "Got status for query:" << status;
        return;
    }

    bdStdPrintNodeId(std::cout, &id->id);
    quint16 port = htons(id->addr.sin_port);

    qDebug() << "FOUND PEER:" << endl;
    qDebug() << inet_ntoa(id->addr.sin_addr) << ":" << port << endl;

    std::stringstream stream;
    bdStdPrintNodeId(stream, &id->id);

    emit peerIpFound(QString::fromStdString(stream.str()),
                     QHostAddress(QString(inet_ntoa(id->addr.sin_addr))), port);
}

bool DhtManager::findNodeByHash(const QByteArray &hash)
{           
    m_pendingQueries.insert(hash);
    m_pendingForQuery.enqueue(hash);
    return true;
}

void DhtManager::dhtNodeCallback(const bdId *node, uint32_t peerflags)
{
    m_nodeCount++;

    dhtNode dnode;
    dnode.id = toByteArray(node);
    dnode.addr = node->addr;

    m_nodeQueue.enqueue(dnode);

    debugDht() << "Discovered node:" << bdIdToString(node) << "with ip:" <<
                  inet_ntoa(node->addr.sin_addr) << peerflags;
}

void DhtManager::dhtValueCallback(const bdId *id, std::string hash, uint32_t status)
{
    qDebug() << "Value found:" << bdIdToString(id) << QString::fromStdString(hash);
}

int DhtManager::nodeCount()
{
    return m_nodeCount;
}

void DhtManager::setOwnIp(const std::string &ip)
{
    m_ownIp = ip;
}

void DhtManager::setupQueryTimer()
{
    m_queryTimer.setInterval(PUSH_INTERVAL);
    connect(&m_queryTimer,&QTimer::timeout, [&]() {

        // Send put messages about our ip and query for ips
        while (!m_nodeQueue.isEmpty()) {
            dhtNode dnode = m_nodeQueue.dequeue();
            const bdId &targetId = toBdId(dnode);
            qDebug() << "Pushing our credentials to node:" << bdIdToString(&targetId) << "with ip" << inet_ntoa(targetId.addr.sin_addr)
                     << "value is" << QString::fromStdString(m_ownIp);

            bdId nonConst = targetId;
            m_udpBitDht->postHash(nonConst, *m_ownId, m_ownIp, SECRET);

            for (const QByteArray &id : m_pendingQueries) {
                bdNodeId key = fromByteArray(id);

//                qDebug() << "Getting hash from node " << bdIdToString(&targetId) << "for key"
//                         << bdNodeIdToString(&key);
                m_udpBitDht->getHash(nonConst, key);
            }

            // If we accidentally stumbled upon a peer we're searching for
            if (m_pendingQueries.contains(toByteArray(&targetId)))
                foundPeer(&targetId, BITDHT_QUERY_SUCCESS);
        }

        // Launch findNode queries for as many nodes as we are allowed
        int index = 0;
        while(!m_pendingForQuery.isEmpty() && index < MAX_QUERY_PER_MINUTE) {
            const QByteArray &hash = m_pendingForQuery.dequeue();

            bdNodeId friendId;
            bdStdNodeIdFromArray(&friendId, hash.data());
            findNode(&friendId);

            index++;
        }

        // Check the file for results
        pollFile();
    });

    m_queryTimer.start();
}

void DhtManager::pollFile()
{
    QFile dhtFile(DHT_FILE);
    if (dhtFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&dhtFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList tokens = line.split(" ");
            if (tokens.size() != 3) {
                continue;
            }
            QString nodeId = tokens.first();
            if (m_pendingQueries.contains(QByteArray::fromHex(nodeId.toUtf8()))) {
                QString host = tokens[1];
                if (tokens[2] == QString::fromStdString(SECRET)) {
                    emit peerIpFound(nodeId, QHostAddress(host), 0);
                    qDebug() << "Peer found:" << nodeId << host;
                }
            }
        }
    }

    dhtFile.close();
    dhtFile.remove();
}
