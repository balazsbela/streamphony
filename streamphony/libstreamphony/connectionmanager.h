#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>

class XmppManager;

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(XmppManager *xmppManager, QObject *parent = 0);
    ~ConnectionManager() override;

private:
    QScopedPointer<XmppManager> m_xmppManager;

};

#endif // CONNECTIONMANAGER_H
