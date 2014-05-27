#include "xmpp/xmppmanager.h"
#include "singleshottimer.h"

#include "QXmppRosterManager.h"
#include "QXmppVCardIq.h"
#include "QXmppVCardManager.h"
#include "QXmppUtils.h"

#include <QDebug>
#include <QStringList>
#include <QCryptographicHash>

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
            m_vCardCache.requestVCard(bareJid);
            //qDebug() << roster.bareJid() <<  roster.name();
        }      
    });

    connect(&m_xmppClient.rosterManager(), &QXmppRosterManager::presenceChanged, [&](const QString& bareJid, const QString& resource) {
        QMap<QString, QXmppPresence> presences = m_xmppClient.rosterManager().getAllPresencesForBareJid(bareJid);
        QXmppPresence& presence = presences[resource];

        const QXmppRosterIq::Item &roster = m_xmppClient.rosterManager().getRosterEntry(bareJid);
        m_presenceHash[roster.bareJid()] = presence;
        if (presence.availableStatusType() == QXmppPresence::Online) {
          //  qDebug() << roster.name() << roster.bareJid() << "Online" << presence.statusText();
        }

        qDebug() << roster.name() << roster.bareJid() << "Online" << presence.statusText() << presence.type() << presence.availableStatusType();

        utils::singleShotTimer(3000, [&]() {
            if (!m_signInCompleted) {
                m_signInCompleted = true;
                emit signInCompleted();
                QXmppPresence presence(QXmppPresence::Available);
                presence.setAvailableStatusType(QXmppPresence::Online);
                m_xmppClient.setClientPresence(presence);
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
        return m_presenceHash[key].availableStatusType() == QXmppPresence::Online;
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
    if (vCard.fullName().isEmpty()) {
        m_vCardCache.requestVCard(bareJid);
        return {};
    }

    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(vCard.photo());

    //const QString userHash = hash.result().toHex();
    //qDebug() << "Returning userUniqueId for " << userHash;

    return hash.result();
}

QString XmppManager::ownJid()
{
    return m_xmppClient.configuration().jidBare();
}

QString XmppManager::fullName(const QString &bareJid)
{
    return m_vCardCache.getVCard(bareJid).fullName();
}

