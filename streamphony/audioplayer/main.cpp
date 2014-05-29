#include "audioplayer/phononmediaplayer.h"
#include "audioplayer/mediastream.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //MediaStream mediaStream

    PhononMediaPlayer player;
    player.play(QStringLiteral("http://79.116.48.163:45172/1.mp3"));

    return a.exec();
}
