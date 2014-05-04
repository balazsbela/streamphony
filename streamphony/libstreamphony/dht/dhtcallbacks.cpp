#include "dhtcallbacks.h"
#include "dhtdebug.h"

DhtCallbacks::DhtCallbacks(DhtManager *parent)
    : BitDhtCallback(),
      m_parent(parent)
{
}

DhtCallbacks::~DhtCallbacks()
{
}

int DhtCallbacks::dhtNodeCallback(const bdId *node, uint32_t peerflags)
{
    debugDht() << "DHT NODE Callback" << endl;
    bdStdPrintNodeId(std::cout, &(node->id));

    return 0;
}

int DhtCallbacks::dhtPeerCallback(const bdId *node, uint32_t status)
{
    debugDht() << "DHT Peer Callback" << status << endl;
    bdStdPrintNodeId(std::cout, &(node->id));
    return 0;
}

int DhtCallbacks::dhtValueCallback(const bdNodeId *id, std::string key, uint32_t status)
{
    debugDht() << "DHT Value Callback" << status << endl;
    bdStdPrintNodeId(std::cout, id);

    debugDht() << endl << "Key:" << QString::fromStdString(key);

    return 0;
}


int DhtCallbacks::dhtConnectCallback(const bdId *srcId, const bdId *proxyId, const bdId *destId,
                                     uint32_t mode, uint32_t point, uint32_t param, uint32_t cbtype,
                                     uint32_t errcode)
{
    debugDht() << "DHT Connect Callback" << endl;
    bdStdPrintNodeId(std::cout, &(srcId->id));
    debugDht() << endl;


    return 0;
}

int DhtCallbacks::dhtInfoCallback(const bdId *node, uint32_t type, uint32_t flags, std::string info)
{
    debugDht() << "DHT Info Callback" << endl;
    bdStdPrintNodeId(std::cout, &(node->id));
    debugDht() << endl;

    return 0;
}


