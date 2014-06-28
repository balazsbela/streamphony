#include "restclient.h"
#include "resolve_overload.h"

#include <QHostAddress>
#include <QNetworkReply>

const QString SEARCH_URL = QStringLiteral("http://%1:%2/search?keyword=%3");

static const QString REST_CHARSET = QByteArrayLiteral("utf-8");


RestClient::RestClient(QObject *parent) :
    QObject(parent)
{
}

RestClient::~RestClient()
{
}

void RestClient::search(const QHostAddress &host, quint32 port, const QString &keyword)
{
    QUrl url(SEARCH_URL.arg(host.toString()).arg(port).arg(keyword));
    QNetworkReply *reply = m_networkAccessManager.get(composeRequest(url));
    QSharedPointer<QNetworkReply> replyPointer(reply);

    connect(replyPointer.data(), utils::resolve_overload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, [this, replyPointer]() {
        qWarning() << "Error occured:" << replyPointer->errorString();
    });

    connect(replyPointer.data(), &QNetworkReply::finished, [=](){

        const QByteArray &content = replyPointer->readAll();
        qDebug() << "Lenght:" << content.size();
        qDebug() << "RECEIVED SEARCH RESULTS RESPONSE:" << QString::fromUtf8(content);
        emit searchResults(QString::fromUtf8(content).split(QRegExp("[\r\n]"),QString::SkipEmptyParts));
    });

    connect(replyPointer.data(), utils::resolve_overload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            [=](const QNetworkReply::NetworkError &error){
        qWarning() << "Error during HTTP request:" << replyPointer->errorString();
    });
}

QNetworkRequest RestClient::composeRequest(const QUrl &path) const
{
    QNetworkRequest request;
    request.setUrl(path);
    request.setRawHeader(QByteArrayLiteral("Accept"), {});
    request.setRawHeader(QByteArrayLiteral("Accept-Charset"), {});
    return request;
}
