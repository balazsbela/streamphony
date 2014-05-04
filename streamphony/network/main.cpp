#include <QCoreApplication>

#include "dht/dhtmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /* startup dht : with a random id! */
    bdNodeId ownId;
    bdStdRandomNodeId(&ownId);

    uint16_t port = 6775;

    DhtManager dht(&ownId, port, QStringLiteral("streamphonydht"), QStringLiteral("bdboot.txt"));

    return a.exec();
}
