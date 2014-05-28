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
#include <QQueue>

struct dhtNode {
    QByteArray id;
    struct sockaddr_in addr;
};

class DhtManager : public QObject
{
    Q_OBJECT
public:
    DhtManager(QObject *parent = 0);
    ~DhtManager() override;

    void start(bdNodeId *ownId, uint16_t port, const QString &appId, const QString &bootstrapfile);
    void setOwnIp(const std::string &ip);

    void enable(bool on);
    void shutdown(); /* blocking call */
    void restart();
    bool isEnabled();
    bool isActive();

    bool findNodeByHash(const QByteArray &hash);
    int nodeCount();

    // Callback handlers
    void foundPeer(const bdId *id, uint32_t status);
    void dhtNodeCallback(const bdId *node, uint32_t peerflags);    
    void dhtValueCallback(const bdId *id, std::string hash, uint32_t status);

signals:
    void peerIpFound(const QString &nodeId, const QHostAddress &ip, const quint16 port);

private:
    void setupQueryTimer();
    void pollFile();

    bool findNode(bdNodeId *peerId);
    bool dropNode(bdNodeId *peerId);

private:
    /* real DHT classes */
    QScopedPointer<UdpStack> m_stack;
    QScopedPointer<UdpBitDht> m_udpBitDht;
    QScopedPointer<bdSpace> m_bdSpace;    

    quint64 m_nodeCount = 0;
    bool m_initialized = 0;
    bdNodeId *m_ownId = nullptr;
    std::string m_ownIp;
    QQueue<dhtNode> m_nodeQueue;
    QQueue<QByteArray> m_pendingForQuery;
    QSet<QByteArray> m_pendingQueries;
    QTimer m_queryTimer;

    friend class DhtCallbacks;
};

#endif // DHTMANAGER_H
