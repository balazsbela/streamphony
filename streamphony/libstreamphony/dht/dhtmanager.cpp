#include "dhtmanager.h"
#include "dhtdebug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

DhtManager::DhtManager(bdNodeId *ownId, uint16_t port, const QString &appId, const QString &bootstrapFile, QObject *parent)
    : QObject(parent)
{   
    debugDht() << "Using id:";
    bdStdPrintNodeId(std::cerr, ownId);
    debugDht() << "Using bootstrap file:" << bootstrapFile;

    /* standard dht behaviour */
    bdDhtFunctions *stdFns = new bdStdDht();

    debugDht() << "Creating UDP DHT:";

    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = 0;
    local.sin_port = htons(port);

    mStack.reset(new UdpStack(local));
    mUdpBitDht.reset(new UdpBitDht(mStack.data(), ownId, appId.toStdString(), bootstrapFile.toStdString(), stdFns));
    mStack->addReceiver(mUdpBitDht.data());

    // Start bitDHT thread
    mUdpBitDht->start();

    // Start DHT
    mUdpBitDht->startDht();
}

void DhtManager::enable(bool on)
{
    if (isEnabled() == on)
        return;

    if (on)
        mUdpBitDht->startDht();
    else
        mUdpBitDht->stopDht();
}

void DhtManager::shutdown()
{
    mUdpBitDht->stopDht(); // blocking call
}

void DhtManager::restart()
{
    mUdpBitDht->stopDht();
    mUdpBitDht->startDht();
}

bool DhtManager::isEnabled()
{
    return mUdpBitDht->stateDht() != 0;
}

bool DhtManager::isActive()
{
    return mUdpBitDht->stateDht() >= BITDHT_MGR_STATE_ACTIVE;
}

DhtManager::~DhtManager()
{
}
