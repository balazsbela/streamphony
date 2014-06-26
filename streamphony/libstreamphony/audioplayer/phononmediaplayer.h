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
    enum AudioState { Stopped = 0, Playing = 1, Paused = 2, Error = 3, Loading = 4 };

    explicit PhononMediaPlayer(QObject *parent = 0);
    ~PhononMediaPlayer() override;
    void play(const QString &url);
    AudioState state() const;

public slots:
    void play();
    void playPause();
    void pause();
    void stop();
    void previous();
    void next();
    void seek(qint64 ms);
    void setVolume( int percentage );

signals:
    void volumeChanged( int volumePercentage);
    void timerMilliSeconds( qint64 msElapsed );
    void timerPercentage( unsigned int percentage);
    void seeked(qint64 ms);
    void currentTrackChanged(const QString &title);

private:
    QScopedPointer<Phonon::MediaObject> m_mediaObject;
    QScopedPointer<Phonon::AudioOutput> m_audioOuput;
    QScopedPointer<Phonon::AudioDataOutput> m_audioDataOutput;
    const Phonon::Path m_audioPath;
    QBuffer m_buffer;
    AudioState m_state;
};

#endif // PHONONMEDIAPLAYER_H
