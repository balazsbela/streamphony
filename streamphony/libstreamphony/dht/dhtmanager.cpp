#include "dhtmanager.h"
#include "dhtdebug.h"
#include "dhtcallbacks.h"
#include "singleshottimer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <memory>

static const int PEER_DISCOVERY_TIMEOUT = 180000; //msecs
static const int RANDOM_PEER_LIMIT = 13;

DhtManager::DhtManager(bdNodeId *ownId, uint16_t port, const QString &appId, const QString &bootstrapFile, QObject *parent)
    : QObject(parent)
{   
    debugDht() << "Using id:";
    bdStdPrintNodeId(std::cerr, ownId);
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

    m_udpBitDht->addFindNode(peerId, BITDHT_QFLAGS_DO_IDLE);

    auto poll = [this, peerId]() {
        struct sockaddr_in peerAddr;
        if (m_udpBitDht->getDhtPeerAddress(peerId, peerAddr)) {
            std::cout << "FOUND PEER:" << endl;
            std::cout << inet_ntoa(peerAddr.sin_addr) << ":" << htons(peerAddr.sin_port) << endl;
            m_pollTimer.stop();
        } else {
            std::cout << "Couldn't find peer yet" << endl;
        }
    };

    QSharedPointer<decltype(poll)> callback(new decltype(poll)(poll));

    m_pollTimer.setInterval(10000);
    connect(&m_pollTimer, &QTimer::timeout, [=]() {
        (*callback)();
    });

    m_pollTimer.start();

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
