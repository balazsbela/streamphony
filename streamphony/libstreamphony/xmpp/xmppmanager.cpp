#include "xmpp/xmppmanager.h"

#include "QXmppRosterManager.h"
#include "QXmppVCardIq.h"
#include "QXmppVCardManager.h"

#include <QDebug>
#include <QStringList>

static const QString XMPP_SERVER = QStringLiteral("chat.facebook.com");

XmppManager::XmppManager(QObject *parent) :
    QObject(parent)
{
}

XmppManager::~XmppManager()
{
}

void XmppManager::signIn()
{
    m_xmppClient.configuration().setHost(XMPP_SERVER);
    m_xmppClient.configuration().setJid(SettingsManager::instance()->xmppUsername() + QStringLiteral("@") + XMPP_SERVER);
    m_xmppClient.configuration().setPassword(SettingsManager::instance()->password());

    m_xmppClient.connectToServer(m_xmppClient.configuration());

    connect(&m_xmppClient.rosterManager(), &QXmppRosterManager::rosterReceived, [this]() {
        for (const QString &bareJid : m_xmppClient.rosterManager().getRosterBareJids()) {
            const QXmppRosterIq::Item &roster = m_xmppClient.rosterManager().getRosterEntry(bareJid);
            Q_UNUSED(roster);
            //qDebug() << roster.bareJid() <<  roster.name();
            m_xmppClient.vCardManager().requestVCard(bareJid);
        }
    });

    connect(&m_xmppClient.rosterManager(), &QXmppRosterManager::presenceChanged, [&](const QString& bareJid, const QString& resource) {
        QMap<QString, QXmppPresence> presences = m_xmppClient.rosterManager().getAllPresencesForBareJid(bareJid);
        QXmppPresence& presence = presences[resource];

        const QXmppRosterIq::Item &roster = m_xmppClient.rosterManager().getRosterEntry(bareJid);

        m_presenceHash[roster.bareJid()] = presence;
        if (presence.type() == QXmppPresence::Available) {
            qDebug() << roster.name() << roster.bareJid() << "Available" << presence.statusText();            
        }

        emit signInCompleted();
    });

    connect(&m_xmppClient.vCardManager(), &QXmppVCardManager::vCardReceived, [&](const QXmppVCardIq &vcard) {
//        qDebug() << "VCARD:" << vcard.from() << vcard.email() << vcard.description() << vcard.birthday()
//                 << vcard.firstName() << vcard.lastName() << vcard.nickName() << vcard.url()
//                 << vcard.organization().title();

//        qDebug() << vcard.photo();

//        for (const QXmppVCardEmail &email : vcard.emails()) {
//            qDebug() << email.address();
//        }
    });

    connect(&m_xmppClient, &QXmppClient::error, [](QXmppClient::Error error) {
        qDebug() << "XMPP Error:" << error;
    });

    connect(&m_xmppClient, &QXmppClient::connected, [](){
        qDebug() << "XMPP Connected:";
    });

    connect(&m_xmppClient, &QXmppClient::disconnected, []() {
        qDebug() << "XPP Disconnected";
    });
}

QStringList XmppManager::allAvailableJids() const
{
    const QList<QString> &keys = m_presenceHash.keys();
    QList<QString>::const_iterator availableJids =  std::find_if(keys.begin(),keys.end(), [&](const QString &key) -> bool {
        return m_presenceHash[key].type() == QXmppPresence::Available;
    });

    QStringList results;
    while (availableJids != keys.end()) {
        results.append(*availableJids);
        ++availableJids;
    }

    return results;
}

const QXmppRosterIq::Item XmppManager::roster(const QString &bareJid)
{
    return m_xmppClient.rosterManager().getRosterEntry(bareJid);
}

