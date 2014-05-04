#ifndef DHTMANAGER_H
#define DHTMANAGER_H

#include "udp/udpstack.h"
#include "udp/udpbitdht.h"
#include "bitdht/bdiface.h"

#include <QObject>
#include <QScopedPointer>

class DhtManager : public QObject
{
    Q_OBJECT
public:
    DhtManager(bdNodeId *ownId, uint16_t port, const QString &appId, const QString &bootstrapfile, QObject *parent = 0);
    ~DhtManager() override;

    void enable(bool on);
    void shutdown(); /* blocking call */
    void restart();
    bool isEnabled();
    bool isActive();

private:
    /* real DHT classes */
    QScopedPointer<UdpStack> mStack;
    QScopedPointer<UdpBitDht> mUdpBitDht;
};

#endif // DHTMANAGER_H
