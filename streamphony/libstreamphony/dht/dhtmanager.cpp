#include "dhtmanager.h"
#include "dhtdebug.h"
#include "dhtcallbacks.h"
#include "singleshottimer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

bool DhtManager::FindNode(bdNodeId *peerId)
{
    std::cerr << "BitDhtHandler::FindNode(";
    bdStdPrintNodeId(std::cerr, peerId);
    std::cerr << ")" << std::endl;

    m_udpBitDht->addFindNode(peerId, BITDHT_QFLAGS_DO_IDLE);

    return true ;
}

bool DhtManager::DropNode(bdNodeId *peerId)
{
    std::cerr << "BitDhtHandler::DropNode(";
    bdStdPrintNodeId(std::cerr, peerId);
    std::cerr << ")" << std::endl;
    std::cerr << std::endl;

    /* remove in peer */
    m_udpBitDht->removeFindNode(peerId);

    return true ;
}

void DhtManager::discoverRandomPeers()
{
    while(1)
    {
        bdNodeId searchId;
        bdStdRandomNodeId(&searchId);

        FindNode(&searchId);

        sleep(180);

        DropNode(&searchId);
    }
}

DhtManager::~DhtManager()
{
}
