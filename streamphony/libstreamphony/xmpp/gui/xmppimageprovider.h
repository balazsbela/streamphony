#ifndef XMPPIMAGEPROVIDER_H
#define XMPPIMAGEPROVIDER_H

#include "xmpp/xmppmanager.h"

#include <QQuickImageProvider>

class XmppImageProvider : public QQuickImageProvider
{
public:
    XmppImageProvider(const XmppManager *parent);
    QImage requestImage(const QString & id, QSize * size, const QSize & requestedSize) override;

private:
    const XmppManager *m_parent;
};


#endif // XMPPIMAGEPROVIDER_H
