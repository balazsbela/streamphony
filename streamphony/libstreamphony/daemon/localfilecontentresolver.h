#ifndef LOCALFILECONTENTRESOLVER_H
#define LOCALFILECONTENTRESOLVER_H

#include "contentresolver.h"

#include <QByteArray>
#include <QStringList>

class LocalFileContentResolver : public ContentResolver
{
    Q_OBJECT
public:
    explicit LocalFileContentResolver(QObject *parent = 0);
    ~LocalFileContentResolver();

    QByteArray resolve(const QString &path, quint64 offset = 0) override;
    QStringList matches(const QString &keyword) override;
};

#endif // LOCALFILECONTENTRESOLVER_H
