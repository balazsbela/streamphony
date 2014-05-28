#ifndef DHTCALLBACKS_H
#define DHTCALLBACKS_H

#include "dhtmanager.h"

#include <udp/udpstack.h>
#include <udp/udpbitdht.h>
#include <bitdht/bdstddht.h>

#include <QPointer>

class DhtCallbacks : public BitDhtCallback
{
public:
    DhtCallbacks(DhtManager *parent);
    virtual ~DhtCallbacks();   

//    virtual int dhtNodeCallback(const bdId *node, uint32_t peerflags) override;
//    virtual int dhtPeerCallback(const bdNodeId *id, uint32_t status) override;
//    virtual int dhtValueCallback(const bdNodeId *id, std::string key, uint32_t status) override;

    virtual int dhtNodeCallback(const bdId *  /*id*/, uint32_t /*peerflags*/) override;
    virtual int dhtPeerCallback(const bdId *id, uint32_t status) override;
    virtual int dhtValueCallback(const bdId *id, std::string key, uint32_t status) override;

    // connection callback. Not required for basic behaviour, but forced for initial development.
    virtual int dhtConnectCallback(const bdId *srcId, const bdId *proxyId, const bdId *destId,
                                   uint32_t mode, uint32_t point, uint32_t param,
                                   uint32_t cbtype, uint32_t errcode) override;

    // Generic Info callback - initially will be used to provide bad peers.
    virtual int dhtInfoCallback(const bdId *id, uint32_t type, uint32_t flags, std::string info) override;

private:
    bdMutex resultsMtx;

    QPointer<DhtManager> m_parent;    
};

#endif // DHTCALLBACKS_H
