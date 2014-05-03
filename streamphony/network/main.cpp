#include <QCoreApplication>

#include "dht/dhtmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DhtManager dhtManager;

    return a.exec();
}
