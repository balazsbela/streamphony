#include "dhtmanager.h"
#include "dhtdebug.h"
#include "dhtcallbacks.h"
#include "singleshottimer.h"
#include "bitdht/bdiface.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <memory>
#include <sstream>

#include <QCryptographicHash>
#include <QTcpSocket>

static const int PEER_DISCOVERY_TIMEOUT = 180000; //msecs
static const int RANDOM_PEER_LIMIT = 13;

static const std::string SECRET = "StreamphonySecret";

static QHostAddress retrieveOwnLocalIp()
{
    QTcpSocket socket;
    socket.connectToHost("8.8.8.8", 53); // google DNS, or something else reliable
    if (socket.waitForConnected()) {
        return socket.localAddress();
    } else {
        qWarning()
            << "could not determine local IPv4 address:"
            << socket.errorString();
    }
    return {};
}

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

//    auto poll = [this, peerId]() -> bool {
//        struct sockaddr_in peerAddr;
//        if (m_udpBitDht->getDhtPeerAddress(peerId, peerAddr)) {
//            qDebug() << "FOUND PEER:" << endl;
//            qDebug() << inet_ntoa(peerAddr.sin_addr) << ":" << htons(peerAddr.sin_port) << endl;

//            std::stringstream stream;
//            bdStdPrintNodeId(stream, peerId);
//            quint16 port = htons(peerAddr.sin_port);
//            emit peerIpFound(QString::fromStdString(stream.str()),
//                             QHostAddress(QString(inet_ntoa(peerAddr.sin_addr))), port);
//            return true;
//        } else {
//            qDebug() << endl << endl << "Couldn't find peer yet!" << endl;
//            return false;
//        }
//    };

//    QSharedPointer<decltype(poll)> callback(new decltype(poll)(poll));
//    QSharedPointer<QTimer> timer(new QTimer(this));

//    timer->setInterval(10000);
//    connect(timer.data(), &QTimer::timeout, [=]() {
//        if ((*callback)())
//            timer->stop();
//    });
//    timer->start();

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
//    auto poll = [&]() -> bool {
//        qDebug() << "Called poll :" << status ;
        bdStdPrintNodeId(std::cout, &id->id);

//        if (m_udpBitDht->getDhtPeerAddress(id, peerAddr) == 1) {
            quint16 port = htons(id->addr.sin_port);

            qDebug() << "FOUND PEER:" << endl;
            qDebug() << inet_ntoa(id->addr.sin_addr) << ":" << port << endl;

            std::stringstream stream;
            bdStdPrintNodeId(stream, &id->id);

            emit peerIpFound(QString::fromStdString(stream.str()),
                             QHostAddress(QString(inet_ntoa(id->addr.sin_addr))), port);
//            return true;
//        } else {
//            qDebug() << "Couldn't get peer address yet!";
//            return false;
//        }
//    };

//    if (!poll()) {
//        QSharedPointer<decltype(poll)> callback(new decltype(poll)(poll));
//        QSharedPointer<QTimer> timer(new QTimer(this));

//        timer->setInterval(1000);
//        connect(timer.data(), &QTimer::timeout, [=]() {
//            if ((*callback)()) {
//                timer->stop();
//            }
//        });
//        timer->start();
//    }
}

bool DhtManager::findNodeByHash(const QByteArray &hash)
{
    bdNodeId friendId;
    bdStdNodeIdFromArray(&friendId, hash.data());
    return findNode(&friendId);
}

void DhtManager::dhtNodeCallback(const bdId *node, uint32_t peerflags)
{
    m_nodeCount++;

    bdId targetId = *node;

// Not from the thread that own the mutex.
//    m_udpBitDht->postHash(targetId, *m_ownId, m_ownIp, SECRET);
//    debugDht() << "Posting hash to new node:" << QString::fromStdString(m_ownIp);
}

int DhtManager::nodeCount()
{
    return m_nodeCount;
}

void DhtManager::setOwnIp(const std::string &ip)
{
    m_ownIp = ip;
}

