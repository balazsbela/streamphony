#ifndef DHTMANAGER_H
#define DHTMANAGER_H

#include "udp/udpstack.h"
#include "udp/udpbitdht.h"
#include "bitdht/bdstddht.h"
#include "bitdht/bdiface.h"

#include <QObject>
#include <QScopedPointer>
#include <QTimer>

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

    bool findNode(bdNodeId *peerId);
    bool dropNode(bdNodeId *peerId);


private:
    /* real DHT classes */
    QScopedPointer<UdpStack> m_stack;
    QScopedPointer<UdpBitDht> m_udpBitDht;
    QScopedPointer<bdSpace> m_bdSpace;
    QTimer m_pollTimer;

};

#endif // DHTMANAGER_H
