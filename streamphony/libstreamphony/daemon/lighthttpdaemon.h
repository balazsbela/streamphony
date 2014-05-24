#ifndef LIGHTHTTPDAEMON_H
#define LIGHTHTTPDAEMON_H

#include "portfwd.h"
#include <QTcpServer>

class ContentResolver;

class LightHttpDaemon : public QTcpServer
{
    Q_OBJECT
public:
    explicit LightHttpDaemon(quint32 port, QObject *parent = 0);
    ~LightHttpDaemon() override;

    void setContentResolver(ContentResolver *contentResolver);

private slots:
    void acceptConnection();
    void handleClient();

private:
    QScopedPointer<ContentResolver> m_contentResolver;
    Portfwd m_portForwarder;
    quint32 m_port;
};

#endif // LIGHTHTTPDAEMON_H
