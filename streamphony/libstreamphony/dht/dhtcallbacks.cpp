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

int DhtCallbacks::dhtPeerCallback(const bdNodeId *id, uint32_t status)
{
    debugDht() << "DHT Peer Callback" << status << endl;
    bdStdPrintNodeId(std::cout, id);

    m_parent->foundPeer(id, status);
    return 0;
}

int DhtCallbacks::dhtValueCallback(const bdNodeId *id, std::string key, uint32_t status)
{
    debugDht() << "DHT Value Callback" << status << endl;
    bdStdPrintNodeId(std::cout, id);

    debugDht() << endl << "Key:" << QString::fromStdString(key);

    return 0;
}
