#include "dht/dhtmanager.h"
#include "xmpp/xmppmanager.h"
#include "settings/settingsmanager.h"

#include "singleshottimer.h"

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SettingsManager settingsManager(&app);
    SettingsManager::instance()->setEmail(QStringLiteral("balazsbela@gmail.com"));
    SettingsManager::instance()->setXmppUsername(QStringLiteral("balazsbela1"));

    XmppManager xmppManager(&app);
    xmppManager.signIn();

    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(SettingsManager::instance()->email().toStdString().c_str());

    bdNodeId ownId;
    bdStdNodeIdFromArray(&ownId, hash.result());

    uint16_t port = 6775;

    DhtManager dht(&ownId, port, QStringLiteral("streamphonydht"), QStringLiteral("bdboot.txt"), &app);

    QCryptographicHash friendHash(QCryptographicHash::Sha1);
    friendHash.addData("balazsbela90@gmail.com");

    bdNodeId friendId;
    bdStdNodeIdFromArray(&friendId, friendHash.result());

    utils::singleShotTimer(15000, [&]() {
        dht.findNode(&friendId);
    }, nullptr);

    return app.exec();
}
