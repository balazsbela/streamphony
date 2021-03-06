#ifndef NODE_H
#define NODE_H

#include "audioplayer/phononmediaplayer.h"
#include "audioplayer/restclient.h"

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QPointer>

//! Encapsulates the api exposed by the running httpdaemon on a friend node
//! Maintains connection

class Node : public QObject
{
    Q_OBJECT
public:
    explicit Node(const QString &id, const QHostAddress &ip, const quint16 port, PhononMediaPlayer *player, QObject *parent = 0);
    ~Node() override;

    QHostAddress host() const;
    quint32 port() const;

    void search(const QString &keyword);
    bool isConnected() const;
    QString id() const;

    void play(const QString &file);

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
    RestClient m_restClient;
    QPointer<PhononMediaPlayer> m_player;
};

#endif // NODE_H
