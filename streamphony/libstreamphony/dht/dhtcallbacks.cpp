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

    m_parent->dhtNodeCallback(node, peerflags);

    return 0;
}

int DhtCallbacks::dhtPeerCallback(const bdId *id, uint32_t status)
{
    debugDht() << "DHT Peer Callback" << status << endl;
    bdStdPrintNodeId(std::cout, &id->id);

    m_parent->foundPeer(&id->id, status);
    return 0;
}

int DhtCallbacks::dhtValueCallback(const bdId *id, std::string key, uint32_t status)
{
    debugDht() << "DHT Value Callback" << status << endl;
    bdStdPrintNodeId(std::cout, &id->id);

    debugDht() << endl << "Key:" << QString::fromStdString(key);

    return 0;
}

int DhtCallbacks::dhtConnectCallback(const bdId *srcId, const bdId *proxyId, const bdId *destId,
                               uint32_t mode, uint32_t point, uint32_t param,
                               uint32_t cbtype, uint32_t errcode)
{
    Q_UNUSED(proxyId);
    Q_UNUSED(destId);
    Q_UNUSED(mode);
    Q_UNUSED(point);
    Q_UNUSED(param);
    Q_UNUSED(cbtype);
    Q_UNUSED(errcode);

    debugDht() << "dhtConnectCallback:";
    bdStdPrintNodeId(std::cout, &srcId->id);
    return 0;
}

int DhtCallbacks::dhtInfoCallback(const bdId *id, uint32_t type, uint32_t flags, std::string info)
{
    Q_UNUSED(flags);
    Q_UNUSED(id);
    Q_UNUSED(type);

    debugDht() << "dhtInfoCallback:" << QString::fromStdString(info);
    return 0;
}
