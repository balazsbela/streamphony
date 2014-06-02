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
    connect(&m_socket, &QTcpSocket::connected, [&]() {
        tryingToConnect = false;
        debugNode() << "Node connected:" << m_id << m_ip;
        const QString url = QStringLiteral("http://") + m_ip.toString() + QStringLiteral(":") + QString::number(m_port) + "/1.mp3";
        debugNode() << url;
    });

    connect(&m_socket, &QTcpSocket::disconnected, [&]() {
        debugNode() << "Node connected:" << m_id << m_ip;
        tryToConnect();
    });

    connect(&m_socket, utils::resolve_overload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), [&]() {
        if (tryingToConnect) {
            if (m_port <= MIN_PORT || m_port >= MAX_PORT) {
                m_port = MIN_PORT;
            }

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

    m_port = port;
    tryToConnect();

    connect(&m_restClient, &RestClient::searchResults, this, &Node::searchResults);
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

void Node::search(const QString &keyword)
{
    m_restClient.search(m_ip, m_port, keyword);
}

bool Node::isConnected() const
{
    return m_socket.state() == QTcpSocket::ConnectedState;
}

QString Node::id() const
{
    return m_id;
}

void Node::play(const QString &file)
{
    m_player.play(QStringLiteral("http://") +
                  m_ip.toString() +
                  QStringLiteral(":") +
                  QString::number(m_port) +
                  QStringLiteral("/") +
                  file);
}
