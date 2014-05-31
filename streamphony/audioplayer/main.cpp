#include "audioplayer/phononmediaplayer.h"
#include "audioplayer/mediastream.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PhononMediaPlayer player;
    //player.play(QStringLiteral("http://79.116.48.163:45172/1.mp3"));
    player.play(QStringLiteral("http://127.0.0.1:45173/1.mp3"));

    return a.exec();
}
