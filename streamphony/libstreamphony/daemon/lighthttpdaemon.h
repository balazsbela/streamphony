#ifndef LIGHTHTTPDAEMON_H
#define LIGHTHTTPDAEMON_H

#ifndef SERVER_ONLY
#include "portfwd.h"
#endif

#include <QTcpServer>

class ContentResolver;

class LightHttpDaemon : public QTcpServer
{
    Q_OBJECT
public:
    explicit LightHttpDaemon(quint32 preferredPort, quint32 minPort, quint32 maxPort, QObject *parent = 0);
    ~LightHttpDaemon() override;

    void setContentResolver(ContentResolver *contentResolver);

private slots:
    void acceptConnection();
    void handleClient();

private:
    QScopedPointer<ContentResolver> m_contentResolver;
#ifndef SERVER_ONLY
    Portfwd m_portForwarder;
#endif
    quint32 m_port = 0;
};

#endif // LIGHTHTTPDAEMON_H
