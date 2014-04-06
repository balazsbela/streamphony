#include "audioplayer.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AudioPlayer player;

    return a.exec();
}
