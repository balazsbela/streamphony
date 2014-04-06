#include "audioplayer.h"
#include "resolve_overload.h"

#include <QFile>

#ifdef DEBUG_AUDIO_PLAYER
#define debugAudioPlayer qDebug
#else
#define debugAudioPlayer QT_NO_QDEBUG_MACRO;
#endif

AudioPlayer::AudioPlayer(QObject *parent)
    : QObject(parent)
    , m_decoder(parent)
    , m_deviceInfo(QAudioDeviceInfo::defaultOutputDevice())
{
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    if (!m_deviceInfo.isFormatSupported(format)) {
        qWarning() << "Audio format not supported!";
        return;
    }

    m_decoder.setAudioFormat(format);

    QFile musicTestFile("1.mp3");
    if (!musicTestFile.exists())
        qWarning() << "Could not find test file!";

    if (!musicTestFile.open(QIODevice::ReadOnly))
        qWarning() << "Could not open test file!";

    m_decoder.setSourceDevice(&musicTestFile);

    m_output.reset(new QAudioOutput(format, this));

    connect(m_output.data(), &QAudioOutput::stateChanged, [&]() {
        if (m_output->error() != QAudio::NoError) {
            qWarning() << "Error occured:" << m_output->error();
            return;
        }

        if (m_output->state() == QAudio::IdleState && m_output->error() == QAudio::NoError) {
            connect(&m_decoder, &QAudioDecoder::bufferReady, [&](){

                if (m_decoder.bufferAvailable()) {
                    const QAudioBuffer buffer = m_decoder.read();
                    debugAudioPlayer() << buffer.format();
                    m_device->write(buffer.constData<char>(), buffer.byteCount());
                }
            });
        }
    });

    connect (&m_decoder, utils::resolve_overload<QAudioDecoder::Error>::of(&QAudioDecoder::error), [&](QAudioDecoder::Error error) {
       debugAudioPlayer() << error << m_decoder.errorString();
    });

    m_device.reset(m_output->start());
    m_decoder.start();

}
