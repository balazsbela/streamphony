#include "lighthttpdaemon.h"
#include "resolve_overload.h"

#include <QTcpSocket>

// HTTP Constants
const QString OK_HEADER = QStringLiteral("HTTP/1.1 200 OK\r\n");
const QString BAD_REQUEST = QStringLiteral("HTTP/1.1 400 Bad Request\r\n\r\n");
const QString CONTENT_LENGTH_HEADER = QStringLiteral("Content-Length: %1 \r\n\r\n");

const int MAX_PENDING_CONNECTIONS = 300; // This is sparta!


LightHttpDaemon::LightHttpDaemon(quint32 port, QObject *parent) :
    QTcpServer(parent)
{
    listen(QHostAddress::Any, port);
    connect(this, &QTcpServer::newConnection, this, &LightHttpDaemon::acceptConnection);
    setMaxPendingConnections(MAX_PENDING_CONNECTIONS);
}

LightHttpDaemon::~LightHttpDaemon()
{
}

void LightHttpDaemon::acceptConnection()
{
    QTcpSocket *socket = nextPendingConnection();

    if (socket == nullptr) {
        qCritical() << "Couldn't get next pending connection!";
        return;
    }

    connect(socket, &QTcpSocket::readyRead, this, &LightHttpDaemon::handleClient);
    connect(socket, utils::resolve_overload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), [&]() {
        qWarning() << "Error occured:" << socket->errorString();
    });

    connect(socket, &QTcpSocket::disconnected, [&]() {
        QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
        if (senderSocket) {
            senderSocket->disconnect(); // all signals
            senderSocket->deleteLater();
        }
        qWarning() << "Connection closed";
    });
}

void LightHttpDaemon::handleClient()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    if (socket == nullptr) {
        qWarning() << "Sender socket null!";
        return;
    }

    if (socket->canReadLine()) {

        const QStringList &tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));

        QTextStream os(socket);
        os.setAutoDetectUnicode(true);

        if (tokens.size() < 3) {
            qWarning() << "Invalid request!";
            os << BAD_REQUEST;
            socket->close();
            return;
        }

        if (tokens[0] == QStringLiteral("GET")) {
            const QString &path = tokens[1];

            os << OK_HEADER;
            os << CONTENT_LENGTH_HEADER.arg("0");
            socket->close();
        }
    }
}
