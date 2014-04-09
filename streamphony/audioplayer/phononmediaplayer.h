#ifndef PHONONMEDIAPLAYER_H
#define PHONONMEDIAPLAYER_H

#include <QObject>
#include <QSharedPointer>
#include <QBuffer>

#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <phonon/AudioDataOutput>
#include <phonon/Path>

#ifdef DEBUG_AUDIO_PLAYER
#define debugPhononMediaPlayer qDebug
#else
#define debugPhononMediaPlayer QT_NO_QDEBUG_MACRO
#endif

class PhononMediaPlayer : public QObject
{
    Q_OBJECT
public:
    explicit PhononMediaPlayer(QObject *parent = 0);
    ~PhononMediaPlayer() override;
    void play();

private:
    QScopedPointer<Phonon::MediaObject> m_mediaObject;
    QScopedPointer<Phonon::AudioOutput> m_audioOuput;
    QScopedPointer<Phonon::AudioDataOutput> m_audioDataOutput;
    const Phonon::Path m_audioPath;
    QBuffer m_buffer;
};

#endif // PHONONMEDIAPLAYER_H
