#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "debug.h"

#include <QObject>
#include <QAudioDecoder>
#include <QAudioOutput>
#include <QAudioDeviceInfo>

class AudioPlayer : public QObject
{
    Q_OBJECT

public:
    AudioPlayer(QObject *parent = 0);


private:
    QAudioDecoder m_decoder;
    QScopedPointer<QAudioOutput> m_output;
    QAudioDeviceInfo m_deviceInfo;
    QScopedPointer<QIODevice> m_device;
};

#endif // AUDIOPLAYER_H
