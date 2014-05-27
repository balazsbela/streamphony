#ifndef XMPPMANAGER_H
#define XMPPMANAGER_H

#include "QXmppClient.h"
#include "settings/settingsmanager.h"
#include "QXmppRosterManager.h"
#include "QXmppVCardIq.h"
#include "vcardcache.h"

#include <QObject>
#include <QSharedPointer>


class XmppManager : public QObject
{
    Q_OBJECT
public:
    explicit XmppManager(QObject *parent = 0);
    ~XmppManager() override;

    void signIn();
    QStringList allAvailableJids() const;
    const QXmppRosterIq::Item roster(const QString &bareJid);
    QByteArray userUniqueId(const QString &bareJid);
    QString ownJid();
    QString fullName(const QString &bareJid);

signals:
    void signInCompleted();

private:
    QXmppClient m_xmppClient;
    QHash<QString, QXmppPresence> m_presenceHash;
    vCardCache m_vCardCache;
    bool m_signInCompleted = false;
};

#endif // XMPPMANAGER_H
