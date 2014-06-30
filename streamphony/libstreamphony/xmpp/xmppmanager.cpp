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
    m_rosterItemSortFilterModel->setSourceModel(&m_rosterItemModel);
}

XmppManager::~XmppManager()
{
}

void XmppManager::signIn(const QString &username, const QString &password, bool rememberPassword)
{
    m_rosterItemModel.clear();

    SettingsManager::instance()->setXmppUsername(username);
    if (rememberPassword)
        SettingsManager::instance()->setPassword(password);
    else
        SettingsManager::instance()->setPassword(QStringLiteral(""));

    m_xmppClient.configuration().setHost(XMPP_SERVER);
    m_xmppClient.configuration().setJid(username + QStringLiteral("@") + XMPP_SERVER);
    m_xmppClient.configuration().setPassword(password);

    connect(&m_xmppClient.rosterManager(), &QXmppRosterManager::presenceChanged, [&](const QString& bareJid, const QString& resource) {

        const QXmppPresence &presence = m_xmppClient.rosterManager().getPresence(bareJid, resource);

        const QXmppRosterIq::Item &roster = m_xmppClient.rosterManager().getRosterEntry(bareJid);

        // -------------------------------------------------
        // Uncomment this when the hack below is removed
        // m_presenceHash[roster.bareJid()] = presence;
        // ----------------------- HAAAAACK------------------------------

        if (presence.type() == QXmppPresence::Available) {
            //  qDebug() << roster.name() << roster.bareJid() << "Online" << presence.statusText();
            m_rosterItemModel.updatePresence(bareJid, {{"", presence}});
            m_rosterItemModel.updateRosterEntry(bareJid, m_xmppClient.rosterManager().getRosterEntry(bareJid));
            qDebug() << roster.name() << roster.bareJid() << "Online" << presence.statusText() << presence.type() << presence.availableStatusType();
        } else {
            m_rosterItemModel.removeRosterEntry(bareJid);
        }

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

    connect(&m_vCardCache,&vCardCache::vCardReadyToUse, [=](const QString &bareJid) {
        updateVCard(bareJid);
    });

    connect(&m_xmppClient, &QXmppClient::error, [](QXmppClient::Error error) {
        qDebug() << "XMPP Error:" << error;
    });

    connect(&m_xmppClient.rosterManager(), &QXmppRosterManager::rosterReceived, [this]() {
        for (const QString &bareJid : m_xmppClient.rosterManager().getRosterBareJids()) {
            const QXmppRosterIq::Item &roster = m_xmppClient.rosterManager().getRosterEntry(bareJid);
            Q_UNUSED(roster);

            m_vCardCache.requestVCard(bareJid);

            // ----------------------- HAAAAACK------------------------------

            // Facebook is retarded and puts me into some weird group and we don't get a presence update
            // TODO: REMOVE AFTER PRESENTATION
            // Restrict queries to these two names to speed things up for the presentation
            if (roster.name().contains("Balázs Béla") || roster.name().contains("Sebestyén Boglárka")) {
                m_presenceHash[roster.bareJid()] = QXmppPresence::Available;
                m_rosterItemModel.updatePresence(bareJid, {{"", QXmppPresence::Available}});
                m_rosterItemModel.updateRosterEntry(bareJid, m_xmppClient.rosterManager().getRosterEntry(bareJid));
            }

            // -----------------------END HAAAAACK------------------------------

            qDebug() << m_xmppClient.rosterManager().getPresence(bareJid, "").type();
        }
    });

    connect(&m_xmppClient, &QXmppClient::connected, [&](){
        emit signInSuccessfull();
        qDebug() << "XMPP Connected:";
        m_vCardCache.requestVCard(m_xmppClient.configuration().jidBare());
    });

    connect(&m_xmppClient, &QXmppClient::disconnected, [&]() {
        qDebug() << "XPP Disconnected";
        m_signInCompleted = false;
    });

    connect(&m_xmppClient.rosterManager(), &QXmppRosterManager::itemChanged, [&] (const QString &bareJid) {

        const QXmppRosterIq::Item &roster = m_xmppClient.rosterManager().getRosterEntry(bareJid);

        if (m_xmppClient.rosterManager().getPresence(bareJid, QStringLiteral("")).type() == QXmppPresence::Available) {
            m_rosterItemModel.updateRosterEntry(bareJid, roster);
        }

        if(m_vCardCache.isVCardAvailable(bareJid))
            updateVCard(bareJid);
    });

    m_xmppClient.connectToServer(m_xmppClient.configuration());
}

QStringList XmppManager::allAvailableJids() const
{
    const QList<QString> &keys = m_presenceHash.keys();
    QList<QString>::const_iterator availableJids =  std::find_if(keys.begin(),keys.end(), [this](const QString &key) -> bool {
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

void XmppManager::updateVCard(const QString &bareJid)
{
    // determine full name
    const QXmppVCardIq vCard = m_vCardCache.getVCard(bareJid);
    QString fullName = vCard.fullName();
    if (fullName.isEmpty())
        fullName = bareJid;

    // determine avatar
    QImage avatar = m_vCardCache.getAvatar(bareJid);

    if (avatar.isNull())
        avatar = QImage();

    if (bareJid != m_xmppClient.configuration().jidBare()) {
        if (m_xmppClient.rosterManager().getPresence(bareJid, QStringLiteral("")).type() == QXmppPresence::Available) {
            // update roster information
            m_rosterItemModel.updateAvatar(bareJid, avatar);
            m_rosterItemModel.updateName(bareJid, fullName);

            emit avatarChanged(bareJid);
        }
    }
}

RosterItemSortFilterProxyModel* XmppManager::model()
{
    return m_rosterItemSortFilterModel;
}

QImage XmppManager::avatar(const QString &bareJid) const
{
   return m_vCardCache.getAvatar(bareJid);
}

bool XmppManager::isSigninCompleted() const
{
    return m_signInCompleted;
}
