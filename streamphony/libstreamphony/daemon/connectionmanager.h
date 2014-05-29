#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "xmpp/xmppmanager.h"
#include "daemon/nodestatus.h"

#include <QObject>
#include <QSharedPointer>
#include <QScopedPointer>

class Node;
class DhtManager;

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(DhtManager *dhtManager, XmppManager *xmppManager, QObject *parent = 0);
    ~ConnectionManager() override;

    void populateNodeHash();

private:
    void saveNodes();
    void loadNodes();

private:
    QScopedPointer<XmppManager> m_xmppManager;
    QScopedPointer<DhtManager> m_dhtManager;

    //! key is the bareJid
    QHash<QString, QSharedPointer<Node>> m_nodeHash;
    QHash<QString, QSharedPointer<NodeStatus>> nodeIdMap;
};

#endif // CONNECTIONMANAGER_H
