#include "dht/dhtmanager.h"
#include "singleshottimer.h"

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData("balazsbela@gmail.com");

    bdNodeId ownId;
    bdStdNodeIdFromArray(&ownId, hash.result());

    uint16_t port = 6775;

    DhtManager dht(&ownId, port, QStringLiteral("streamphonydht"), QStringLiteral("bdboot.txt"));


    QCryptographicHash friendHash(QCryptographicHash::Sha1);
    friendHash.addData("balazsbela90@gmail.com");

    bdNodeId friendId;
    bdStdNodeIdFromArray(&friendId, friendHash.result());

    utils::singleShotTimer(15000, [&]() {
        dht.findNode(&friendId);
    }, nullptr);

    return a.exec();
}
