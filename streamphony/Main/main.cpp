#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <QMediaPlayer>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/main.qml"));
    viewer.showExpanded();

    QMediaPlayer mediaPlayer;

    mediaPlayer.setMedia(QUrl("http://192.168.0.102:8000/mpd.mp3"));
    mediaPlayer.setVolume(100);
    mediaPlayer.play();

    return app.exec();
}
