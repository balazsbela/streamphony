#include "phononmediaplayer.h"
#include "singleshottimer.h"
#include "mediastream.h"

#include <QFile>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

PhononMediaPlayer::PhononMediaPlayer(QObject *parent)
    : QObject(parent)
    , m_mediaObject(new Phonon::MediaObject(this))
    , m_audioOuput(new Phonon::AudioOutput(Phonon::MusicCategory, this))
    , m_audioDataOutput(new Phonon::AudioDataOutput(this))
    , m_audioPath(Phonon::createPath(m_mediaObject.data(), m_audioOuput.data()))
{
    if (!m_buffer.open(QIODevice::Append | QIODevice::ReadWrite)) {
        qWarning() << "Could not open buffer for write!";
    }

    m_mediaObject->setCurrentSource(Phonon::MediaSource(&m_buffer));
}

void PhononMediaPlayer::play() {
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request;

//    [balazsbela@archlinux ~]$ cat netcat.sh
//    #!/bin/bash

//    echo HTTP/1.1 200 OK
//    echo Content-Length: 10485760
//    echo
//    cat /home/balazsbela/devel/streamphony/build-streamphony-CLANG-Debug/audioplayer/1.mp3
// [balazsbela@archlinux ~]$ ./netcat.sh |nc -v -l -p 8081

    const QUrl url(QStringLiteral("http://127.0.0.1:8081/1.mp3"));

    request.setUrl(url);

    request.setHeader(QNetworkRequest::UserAgentHeader, QStringLiteral("VLC/2.1.4 LibVLC/2.1.4"));
    request.setRawHeader(QByteArrayLiteral("Range"), QByteArrayLiteral("bytes=0-"));
    request.setRawHeader(QByteArrayLiteral("Connection"), QByteArrayLiteral("Keep-Alive"));
    request.setRawHeader(QByteArrayLiteral("Icy-MetaData"), QByteArrayLiteral("0"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, {});
    request.setRawHeader(QByteArrayLiteral("Accept-Encoding"), {});
    request.setRawHeader(QByteArrayLiteral("Accept-Language"), {});

    QNetworkReply *reply = networkManager->get(request);
    MediaStream *mediaStream = new MediaStream(QSharedPointer<QNetworkReply>(reply), this);

    m_mediaObject->setCurrentSource(mediaStream);
    m_mediaObject->play();
}

PhononMediaPlayer::~PhononMediaPlayer()
{
    m_mediaObject->stop();
}
