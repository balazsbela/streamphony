#ifndef MEDIASTREAM_H
#define MEDIASTREAM_H

#include <phonon/abstractmediastream.h>
#include <QByteArray>
#include <QNetworkReply>
#include <QSharedPointer>
#include <QScopedPointer>

class QIODevice;
class QTimer;

class MediaStream : public Phonon::AbstractMediaStream
{
    Q_OBJECT
public:
    explicit MediaStream(const QSharedPointer<QNetworkReply> &reply, QObject *parent = 0);
    ~MediaStream() override;

    virtual void enoughData() override;
    virtual void needData() override;
    virtual void reset() override;
    virtual void seekStream(qint64 offset) override;

private slots:
    void feedData();

private:
    Q_DISABLE_COPY(MediaStream)

    QByteArray m_data;
    QSharedPointer<QNetworkReply> m_networkReply;
    qint64 m_position;
    QScopedPointer<QTimer> m_timer;
};

#endif // MEDIASTREAM_H
