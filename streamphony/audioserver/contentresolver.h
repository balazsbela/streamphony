#ifndef CONTENTRESOLVER_H
#define CONTENTRESOLVER_H

#include <QObject>

//! Abstract base class for resolving music from various sources

class ContentResolver : public QObject
{
    Q_OBJECT
public:
    ContentResolver(QObject *parent = 0) : QObject(parent) {};
    virtual ~ContentResolver() {};

    virtual QByteArray resolve(const QString &path, quint64 offset = 0) = 0;
    virtual QStringList matches(const QString &keyword) = 0;

    Q_DISABLE_COPY(ContentResolver)
};

#endif // CONTENTRESOLVER_H
