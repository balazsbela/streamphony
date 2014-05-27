#ifndef DHTMANAGER_H
#define DHTMANAGER_H

#include "udp/udpstack.h"
#include "udp/udpbitdht.h"
#include "bitdht/bdstddht.h"
#include "bitdht/bdiface.h"

#include <QObject>
#include <QScopedPointer>
#include <QTimer>
#include <QHostAddress>
#include <QHash>

class DhtManager : public QObject
{
    Q_OBJECT
public:
    DhtManager(QObject *parent = 0);
    ~DhtManager() override;

    void start(bdNodeId *ownId, uint16_t port, const QString &appId, const QString &bootstrapfile);

    void enable(bool on);
    void shutdown(); /* blocking call */
    void restart();
    bool isEnabled();
    bool isActive();

    bool findNode(bdNodeId *peerId);
    bool dropNode(bdNodeId *peerId);

    bool findNodeByHash(const QByteArray &hash);
    int nodeCount();

    // Callback handlers
    void foundPeer(const bdNodeId *id, uint32_t status);
    void dhtNodeCallback(const bdId *node, uint32_t peerflags);

signals:
    void peerIpFound(const QString &nodeId, const QHostAddress &ip, const quint16 port);

private:
    /* real DHT classes */
    QScopedPointer<UdpStack> m_stack;
    QScopedPointer<UdpBitDht> m_udpBitDht;
    QScopedPointer<bdSpace> m_bdSpace;    
    quint64 m_nodeCount = 0;
    bool m_initialized = 0;
};

#endif // DHTMANAGER_H
