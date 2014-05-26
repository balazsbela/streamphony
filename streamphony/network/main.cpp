#include "dht/dhtmanager.h"
#include "xmpp/xmppmanager.h"
#include "settings/settingsmanager.h"
#include "daemon/lighthttpdaemon.h"
#include "daemon/localfilecontentresolver.h"
#include "singleshottimer.h"
#include "daemon/portrange.h"
#include "daemon/connectionmanager.h"

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDebug>

static const int MINIMAL_NUMBER_OF_NODES = 5;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SettingsManager settingsManager(&app);
//    SettingsManager::instance()->setEmail(QStringLiteral("balazsbela@gmail.com"));
//    SettingsManager::instance()->setXmppUsername(QStringLiteral("balazsbela1"));

    XmppManager xmppManager(&app);   

    LocalFileContentResolver *resolver = new LocalFileContentResolver(&app);
    LightHttpDaemon daemon(MIN_PORT, MAX_PORT, &app);
    daemon.setContentResolver(resolver);

    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(SettingsManager::instance()->email().toStdString().c_str());

    bdNodeId ownId;
    bdStdNodeIdFromArray(&ownId, hash.result());

    uint16_t port = 6775;

    DhtManager dht(&ownId, port, QStringLiteral("streamphonydht"), QStringLiteral("bdboot.txt"), &app);
    ConnectionManager connectionManager(&dht, &xmppManager, &app);

    QTimer dhtReadyTimer;
    dhtReadyTimer.setInterval(5000);
    QObject::connect(&dhtReadyTimer, &QTimer::timeout, [&]() {
        if (dht.nodeCount() > MINIMAL_NUMBER_OF_NODES) {
            xmppManager.signIn();
            dhtReadyTimer.stop();
        }
    });
    dhtReadyTimer.start();

//    QCryptographicHash friendHash(QCryptographicHash::Sha1);
//    friendHash.addData("balazsbela90@gmail.com");

//    bdNodeId friendId;
//    bdStdNodeIdFromArray(&friendId, friendHash.result());

//    utils::singleShotTimer(15000, [&]() {
//        dht.findNode(&friendId);
//    }, nullptr);

    return app.exec();
}
