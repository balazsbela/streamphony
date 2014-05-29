#include "mediastream.h"
#include "resolve_overload.h"

#include <QTimer>

// Block size of 1MB
#define BLOCK_SIZE 1048576

MediaStream::MediaStream(const QSharedPointer<QNetworkReply> &reply, QObject *parent)
    : Phonon::AbstractMediaStream(parent)
    , m_networkReply(reply)
    , m_position(0)
    , m_timer(new QTimer(this))
{
    if ( !m_networkReply->isOpen() ) {
        m_networkReply->open(QIODevice::ReadOnly);
    }

    Q_ASSERT( m_networkReply->isOpen() );
    Q_ASSERT( m_networkReply->isReadable() );

    connect(m_networkReply.data(), utils::resolve_overload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), [&]() {
       qWarning() << "Network error while streaming:" << m_networkReply->errorString();
    });

    if (m_networkReply->isFinished()) {
        m_data = m_networkReply->readAll();
        Q_ASSERT(m_networkReply->atEnd());
        setStreamSeekable(true);
        setStreamSize(m_data.size());
    } else {
        QVariant contentLength = m_networkReply->header(QNetworkRequest::ContentLengthHeader);

        if (contentLength.isValid() && contentLength.toLongLong() > 0)
            setStreamSize(contentLength.toLongLong());

        m_data = m_networkReply->readAll();
        connect(m_networkReply.data(), &QNetworkReply::readyRead, [&]() {
            m_data += m_networkReply->readAll();
            qDebug() << m_data.size();
        });
    }

    // Infinite loop-like mechanism, but will be stopped when enoughData is called.
    m_timer->setInterval(0);
    connect(m_timer.data(), &QTimer::timeout, this, &MediaStream::feedData);
}

MediaStream::~MediaStream()
{
}

//! This method is responsible for writing the received data into the QIODevice
void MediaStream::feedData()
{
    const QByteArray data = m_data.mid( m_position, BLOCK_SIZE );
    m_position += data.size();

    if (data.size() == 0 && m_networkReply->atEnd() && m_networkReply->isFinished()) {
        endOfData();
        m_timer->stop();
    } else {
        writeData(data);
    }
}

void MediaStream::needData()
{
    m_timer->start();
}

void MediaStream::enoughData()
{
    m_timer->stop();
}

void MediaStream::reset()
{
    m_position = 0;
}

void MediaStream::seekStream(qint64 offset)
{
    m_position = offset;
}



