#ifndef XMPPMANAGER_H
#define XMPPMANAGER_H

#include "QXmppClient.h"
#include <QObject>


class XmppManager : public QObject
{
    Q_OBJECT
public:
    explicit XmppManager(QObject *parent = 0);

private:
    QXmppClient m_xmppClient;

};

#endif // XMPPMANAGER_H
