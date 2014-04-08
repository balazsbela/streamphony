#include "phononmediaplayer.h"
#include "audioplayer.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PhononMediaPlayer player;
    player.play();

    return a.exec();
}
