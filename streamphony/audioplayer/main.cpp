#include "phononmediaplayer.h"
#include "mediastream.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //MediaStream mediaStream

    PhononMediaPlayer player;
    player.play();

    return a.exec();
}
