#include "localfilecontentresolver.h"

#include <QFile>
#include <QDir>
#include <QDebug>

const QStringList FILE_TYPES = {QStringLiteral("*.mp3"),
                                QStringLiteral("*.ogg")};

LocalFileContentResolver::LocalFileContentResolver(QObject *parent) :
    ContentResolver(parent)
{
}

LocalFileContentResolver::~LocalFileContentResolver()
{
}

QByteArray LocalFileContentResolver::resolve(const QString &path, quint64 offset)
{
    QFile file(path);
    QByteArray results;

    if (!file.exists())
        return results;

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file at :" << path;
        return results;
    }

    // FIXME: check path, to make sure it's in a shared folder.
    file.seek(offset);
    results = file.readAll();
    file.close();

    return results;
}

QStringList LocalFileContentResolver::matches(const QString &keyword)
{
    QStringList nameFilters = FILE_TYPES;

    QString searchKey = keyword;
    searchKey.truncate(searchKey.lastIndexOf('.'));
    searchKey += QChar('*');

    nameFilters << searchKey;

    qDebug() << nameFilters;

    QStringList files = QDir::current().entryList(nameFilters,
                                                  QDir::Files | QDir::AllDirs |
                                                  QDir::NoDotAndDotDot | QDir::Readable);

    qDebug() << files;

    return files;
}


