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

    connect(m_mediaObject.data(), &Phonon::MediaObject::stateChanged, this, [this](Phonon::State newState, Phonon::State oldState) {
        Q_UNUSED(oldState);

        switch (newState) {
        case Phonon::LoadingState : {
            m_state = Loading;
            break;
        }
        case Phonon::BufferingState : {
            qWarning() << "Underrun!";
            break;
        }
        case Phonon::PlayingState : {
            m_state =  Playing;
            break;
        }
        case Phonon::StoppedState : {
            m_state = Stopped;
            break;
        }
        case Phonon::PausedState : {
            m_state = Paused;
            break;
        }
        default:
            break;
        }
    });

    connect(m_mediaObject.data(), &Phonon::MediaObject::tick, this, [this](qint64 time) {
        emit timerMilliSeconds(time);

        emit timerPercentage( (double) time / (double) m_mediaObject->totalTime() * 100.0);
    });

}

void PhononMediaPlayer::play(const QString &urlString) {
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request;

    //    [balazsbela@archlinux ~]$ cat netcat.sh
    //    #!/bin/bash

    //    echo HTTP/1.1 200 OK
    //    echo Content-Length: 10485760
    //    echo
    //    cat /home/balazsbela/devel/streamphony/build-streamphony-CLANG-Debug/audioplayer/1.mp3
    // [balazsbela@archlinux ~]$ ./netcat.sh |nc -v -l -p 8081

    const QUrl url(urlString);

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

    if (urlString.contains("/")) {
        emit currentTrackChanged(urlString.right(urlString.lastIndexOf("/")));
    } else {
        emit currentTrackChanged(urlString);
    }
}

PhononMediaPlayer::~PhononMediaPlayer()
{
    m_mediaObject->stop();
}

void PhononMediaPlayer::play()
{
    m_mediaObject->play();
}

void PhononMediaPlayer::playPause()
{
    if (m_state == Paused)
        play();
    else
        pause();
}

void PhononMediaPlayer::pause()
{
    m_mediaObject->pause();
}

void PhononMediaPlayer::stop()
{
    m_mediaObject->stop();
}

void PhononMediaPlayer::previous()
{
    //TODO:
}

void PhononMediaPlayer::next()
{
    //TODO:
}

void PhononMediaPlayer::seek(qint64 ms)
{
    m_mediaObject->seek(ms);
    emit seeked(ms);
}


void PhononMediaPlayer::setVolume( int percentage )
{
    m_audioOuput->setVolume(percentage);
    emit volumeChanged(percentage);
}

PhononMediaPlayer::AudioState PhononMediaPlayer::state() const
{
    return m_state;
}


