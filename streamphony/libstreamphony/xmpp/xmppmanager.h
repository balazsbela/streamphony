#ifndef XMPPMANAGER_H
#define XMPPMANAGER_H

#include "QXmppClient.h"
#include "settings/settingsmanager.h"
#include "QXmppRosterManager.h"
#include "QXmppVCardIq.h"
#include "vcardcache.h"
#include "gui/rosterItem.h"
#include "gui/rosterItemSortFilterProxyModel.h"
#include "gui/rosterItemModel.h"

#include <QObject>
#include <QSharedPointer>

class XmppManager : public QObject
{
    Q_OBJECT
public:
    explicit XmppManager(QObject *parent = 0);
    ~XmppManager() override;

    Q_INVOKABLE void signIn(const QString &username, const QString &password, bool rememberPassword);
    void signIn();
    QStringList allAvailableJids() const;
    const QXmppRosterIq::Item roster(const QString &bareJid);
    QByteArray userUniqueId(const QString &bareJid);
    QString ownJid();
    Q_INVOKABLE QString fullName(const QString &bareJid);
    RosterItemSortFilterProxyModel* model();
    QImage avatar(const QString &bareJid) const;
    bool isSigninCompleted() const;

signals:
    void signInSuccessfull();
    void signInCompleted();
    void avatarChanged(const QString &bareJid);

private:
    void updateVCard(const QString &bareJid);

private:
    QXmppClient m_xmppClient;
    QHash<QString, QXmppPresence> m_presenceHash;
    vCardCache m_vCardCache;
    bool m_signInCompleted = false;

    // GUI Stuff
    RosterItemModel m_rosterItemModel;
    RosterItemSortFilterProxyModel* m_rosterItemSortFilterModel = new RosterItemSortFilterProxyModel();
};

#endif // XMPPMANAGER_H
