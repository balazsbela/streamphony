#include "xmpp/xmppmanager.h"
#include "singleshottimer.h"

#include "QXmppRosterManager.h"
#include "QXmppVCardIq.h"
#include "QXmppVCardManager.h"
#include "QXmppUtils.h"

#include <QDebug>
#include <QStringList>

static const QString XMPP_SERVER = QStringLiteral("chat.facebook.com");

XmppManager::XmppManager(QObject *parent) :
    QObject(parent),
    m_vCardCache(&m_xmppClient)
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
           m_vCardCache.requestVCard(bareJid);
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

        utils::singleShotTimer(3000, [&]() {
            if (!m_signInCompleted) {
                m_signInCompleted = true;
                emit signInCompleted();
            }
        }, this);
    });

    connect(&m_xmppClient.vCardManager(), &QXmppVCardManager::vCardReceived, &m_vCardCache, &vCardCache::vCardReceived);

    connect(&m_xmppClient, &QXmppClient::error, [](QXmppClient::Error error) {
        qDebug() << "XMPP Error:" << error;
    });

    connect(&m_xmppClient, &QXmppClient::connected, [&](){
        qDebug() << "XMPP Connected:";
        m_vCardCache.requestVCard(m_xmppClient.configuration().jidBare());        
    });

    connect(&m_xmppClient, &QXmppClient::disconnected, [&]() {
        qDebug() << "XPP Disconnected";
        m_signInCompleted = false;
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

QByteArray XmppManager::userUniqueId(const QString &bareJid)
{
    auto vCard = m_vCardCache.getVCard(bareJid);
    return vCard.firstName().toUtf8() + vCard.lastName().toUtf8() + vCard.photo();
}

QString XmppManager::ownJid()
{
    return m_xmppClient.configuration().jidBare();
}
