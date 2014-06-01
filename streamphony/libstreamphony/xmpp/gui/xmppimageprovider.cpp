#include "xmppimageprovider.h"

XmppImageProvider::XmppImageProvider(const XmppManager *parent) : QQuickImageProvider(QQuickImageProvider::Image)
{
    Q_ASSERT(parent);
    m_parent = parent;
}

QImage XmppImageProvider::requestImage(const QString & id, QSize * size, const QSize & requestedSize)
{
    Q_ASSERT(m_parent);
    QImage image = m_parent->avatar(id);
    if (image.byteCount() == 0) {
        return QImage(":///gui/icons/profile.png");
    }

    return image;
}
