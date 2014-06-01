#ifndef NODE_H
#define NODE_H

#include "audioplayer/phononmediaplayer.h"
#include "audioplayer/restclient.h"

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

    QHostAddress host() const;
    quint32 port() const;

    void search(const QString &keyword);
    bool isConnected() const;
    QString id() const;

signals:
    void searchResults(const QStringList &results);

private:
    void tryToConnect();

private:
    QString m_id;
    QHostAddress m_ip;
    QTcpSocket m_socket;
    quint32 m_port = 0;
    bool tryingToConnect = false;
    PhononMediaPlayer m_player;
    RestClient m_restClient;
};

#endif // NODE_H
