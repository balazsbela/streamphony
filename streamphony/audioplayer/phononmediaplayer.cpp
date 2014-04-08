#include "phononmediaplayer.h"
#include "singleshottimer.h"

#include <QFile>

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
    QFile testFile("1.mp3");
    if (!testFile.exists())
        qWarning() << "Could not find test file!";

    if (!testFile.open(QIODevice::ReadOnly))
        qWarning() << "Could not open test file!";

    const QByteArray entireFile = testFile.readAll();
    m_buffer.write(entireFile);

    m_mediaObject->play();
}

PhononMediaPlayer::~PhononMediaPlayer()
{
    m_mediaObject->stop();
}
