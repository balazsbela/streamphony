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

    virtual int dhtNodeCallback(const bdId *node, uint32_t peerflags) override;
    virtual int dhtPeerCallback(const bdNodeId *id, uint32_t status) override;
    virtual int dhtValueCallback(const bdNodeId *id, std::string key, uint32_t status) override;

private:
    QPointer<DhtManager> m_parent;
};

#endif // DHTCALLBACKS_H
