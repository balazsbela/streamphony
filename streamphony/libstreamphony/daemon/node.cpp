#include "daemon/node.h"
#include "daemon/portrange.h"
#include "resolve_overload.h"

#define DEBUG_NODE

#ifdef DEBUG_NODE
#define debugNode qDebug
#else
#define debugNode QT_NO_QDEBUG_MACRO
#endif

Node::Node(const QString &id, const QHostAddress &ip, const quint16 port, QObject *parent) :
    QObject(parent),
    m_id(id),
    m_ip(ip)
{
    // It's useless, since it's a bittorrent port or something similar
    Q_UNUSED(port);

    connect(&m_socket, &QTcpSocket::connected, [&]() {
        tryingToConnect = false;
        debugNode() << "Node connected:" << m_id << m_ip;
    });

    connect(&m_socket, &QTcpSocket::disconnected, [&]() {
        debugNode() << "Node connected:" << m_id << m_ip;
        tryToConnect();
    });

    connect(&m_socket, utils::resolve_overload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), [&]() {
        if (tryingToConnect) {
            if (m_port < MAX_PORT) {
                m_port++;
                debugNode() << "Error on socket, retrying";

            } else {
                debugNode() << "Exhausted port range, node must be offline, or wrong dht hit";
            }
        } else {
            debugNode() << "Socket error occured!";
        }
    });

    m_port = MIN_PORT;
    tryToConnect();
}

Node::~Node()
{
}

void Node::tryToConnect()
{
    if (m_socket.state() != QTcpSocket::ConnectedState) {
        debugNode() << "Trying to connect on port:" << m_port;
        tryingToConnect = true;
        m_socket.connectToHost(m_ip, m_port);
    }
}

QHostAddress Node::host() const {
    return m_ip;
}

quint32 Node::port() const {
    return m_port;
}
