#ifndef LOCALFILECONTENTRESOLVER_H
#define LOCALFILECONTENTRESOLVER_H

#include "contentresolver.h"

#include <QByteArray>
#include <QStringList>
#include <QList>
#include <QDir>

class LocalFileContentResolver : public ContentResolver
{
    Q_OBJECT
public:
    explicit LocalFileContentResolver(QObject *parent = 0);
    ~LocalFileContentResolver();

    QByteArray resolve(const QString &path, quint64 offset = 0) override;
    QStringList matches(const QString &keyword) override;
    void length(const QString &path) override;
    void find (const QString &keyword, const QDir &rootFolder, QStringList *results);

private:
    QList<QDir> m_sharedFolders;

};

#endif // LOCALFILECONTENTRESOLVER_H
