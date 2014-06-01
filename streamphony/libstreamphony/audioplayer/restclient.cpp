#include "restclient.h"

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
    connect(reply, &QNetworkReply::isFinished, [&](){
        const QByteArray &content = reply->readAll();

        emit searchResults(QString::fromUtf8(content).split(QRegExp("[\r\n]"),QString::SkipEmptyParts));
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
