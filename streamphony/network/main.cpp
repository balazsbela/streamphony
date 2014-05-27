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
#include <QHostAddress>
#include <QNetworkReply>
#include <QNetworkAccessManager>

static const int MINIMAL_NUMBER_OF_NODES = 2;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SettingsManager settingsManager(&app);
    //    SettingsManager::instance()->setEmail(QStringLiteral("balazsbela@gmail.com"));
    //    SettingsManager::instance()->setXmppUsername(QStringLiteral("balazsbela1"));

    DhtManager dht(&app);


    // Ridiculous, but couldn't find a simpler and faster way to find out the external ip of a router
    QNetworkAccessManager accessManager;
    QNetworkReply *networkReply = accessManager.get(QNetworkRequest(QUrl("http://ipecho.net/plain")));
    QObject::connect(networkReply, &QNetworkReply::finished, [&]() {
        if (networkReply->error() == QNetworkReply::NoError)
            dht.setOwnIp(QString::fromUtf8(networkReply->readAll()).toStdString());
        else
            qWarning() << "ERROR GETTING EXTERNAL IP!";
    });

    XmppManager xmppManager(&app);
    xmppManager.signIn();

    LocalFileContentResolver *resolver = new LocalFileContentResolver(&app);
    LightHttpDaemon daemon(MIN_PORT, MAX_PORT, &app);
    daemon.setContentResolver(resolver);

    uint16_t port = 6775;

    QObject::connect(&xmppManager, &XmppManager::signInCompleted, [&]() {

        const QByteArray &hash = xmppManager.userUniqueId(xmppManager.ownJid());

        qDebug() << "OwnId:" << hash.toHex();

        bdNodeId ownId;
        bdStdNodeIdFromArray(&ownId, hash);
        dht.start(&ownId, port, QStringLiteral("streamphonydht"), QStringLiteral("bdboot.txt"));
    });

    ConnectionManager connectionManager(&dht, &xmppManager, &app);

    QTimer dhtReadyTimer;
    dhtReadyTimer.setInterval(10000);
    QObject::connect(&dhtReadyTimer, &QTimer::timeout, [&]() {
        if (dht.nodeCount() >= MINIMAL_NUMBER_OF_NODES) {
            connectionManager.populateNodeHash();

            // Launch new queries every 30 seconds because online contacts have probably changed.
            dhtReadyTimer.setInterval(30000);
        }
    });
    dhtReadyTimer.start();

    return app.exec();
}
