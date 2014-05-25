#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>

//! Encapsulates the api exposed by the running httpdaemon on a friend node
//! Maintains connection

class Node : public QObject
{
    Q_OBJECT
public:
    explicit Node(const QString &id, const QHostAddress &ip, const quint16 port, QObject *parent = 0);
    ~Node() override;

private:
    void tryToConnect();

private:
    QString m_id;
    QHostAddress m_ip;
    QTcpSocket m_socket;
    quint32 m_port = 0;
    bool tryingToConnect = false;
};

#endif // NODE_H
