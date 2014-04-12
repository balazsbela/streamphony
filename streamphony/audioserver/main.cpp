#include "lighthttpdaemon.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    LightHttpDaemon daemon(8081);

    return a.exec();
}
