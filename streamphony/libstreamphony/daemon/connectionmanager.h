#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "xmpp/xmppmanager.h"
#include "daemon/nodestatus.h"
#include "daemon/gui/searchresultmodel.h"

#include <QObject>
#include <QSharedPointer>
#include <QScopedPointer>
#include <QPointer>

class Node;
class DhtManager;
class PhononMediaPlayer;

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(PhononMediaPlayer *mediaPlayer, DhtManager *dhtManager, XmppManager *xmppManager, QObject *parent = 0);
    ~ConnectionManager() override;

    void populateNodeHash();

    Q_INVOKABLE void searchNodes(const QString &keyword);
    Q_INVOKABLE void play(const QString &fileName, const QString &bareJid);
    Q_INVOKABLE bool isDiscovered(const QString &jid);
    Q_INVOKABLE void listNode(const QString &bareJid);

    ListModel* model();

signals:
    void searchResults(const QStringList &result, const QString &bareJid);

private:
    void saveNodes();
    void loadNodes();

private:
    QPointer<PhononMediaPlayer> m_mediaPlayer;
    QPointer<XmppManager> m_xmppManager;
    QPointer<DhtManager> m_dhtManager;

    //! key is the bareJid
    QHash<QString, QSharedPointer<Node>> m_nodeHash;
    QHash<QString, QSharedPointer<NodeStatus>> nodeIdMap;

    ListModel *m_model = new ListModel(new SearchResultItem());
};

#endif // CONNECTIONMANAGER_H
