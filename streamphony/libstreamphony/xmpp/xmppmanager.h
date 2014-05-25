#ifndef XMPPMANAGER_H
#define XMPPMANAGER_H

#include "QXmppClient.h"
#include "settings/settingsmanager.h"
#include "QXmppRosterManager.h"

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

signals:
    void signInCompleted();

private:
    QXmppClient m_xmppClient;
    QHash<QString, QXmppPresence> m_presenceHash;
};

#endif // XMPPMANAGER_H
