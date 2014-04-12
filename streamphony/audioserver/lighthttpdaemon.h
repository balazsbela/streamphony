#ifndef LIGHTHTTPDAEMON_H
#define LIGHTHTTPDAEMON_H

#include <QTcpServer>

class LightHttpDaemon : public QTcpServer
{
    Q_OBJECT
public:
    explicit LightHttpDaemon(quint32 port, QObject *parent = 0);
    ~LightHttpDaemon() override;

private slots:
    void acceptConnection();
    void handleClient();
};

#endif // LIGHTHTTPDAEMON_H
