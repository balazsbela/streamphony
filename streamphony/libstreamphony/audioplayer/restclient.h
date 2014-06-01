#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>

class RestClient : public QObject
{
    Q_OBJECT
public:
    explicit RestClient(QObject *parent = 0);
    ~RestClient() override;

    void search(const QHostAddress &host, quint32 port, const QString &keyword);

signals:
    void searchResults(const QStringList &results);

private:
     QNetworkRequest composeRequest(const QUrl &path) const;

private:
    QNetworkAccessManager m_networkAccessManager;

};

#endif // RESTCLIENT_H
