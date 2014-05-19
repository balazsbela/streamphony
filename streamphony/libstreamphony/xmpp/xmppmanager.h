#ifndef XMPPMANAGER_H
#define XMPPMANAGER_H

#include "QXmppClient.h"
#include "settings/settingsmanager.h"

#include <QObject>


class XmppManager : public QObject
{
    Q_OBJECT
public:
    explicit XmppManager(QObject *parent = 0);
    ~XmppManager() override;

    void signIn();

private:
    QXmppClient m_xmppClient;

};

#endif // XMPPMANAGER_H
