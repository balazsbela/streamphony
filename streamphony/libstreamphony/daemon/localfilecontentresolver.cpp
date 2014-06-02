#include "localfilecontentresolver.h"
#include "settings/settingsmanager.h"

#include <QFile>
#include <QDir>
#include <QDebug>

const QStringList FILE_TYPES = {QStringLiteral("*%1.mp3"),
                                QStringLiteral("*%1.ogg")};

LocalFileContentResolver::LocalFileContentResolver(QObject *parent) :
    ContentResolver(parent)
{
    for (const QString &folder : SettingsManager::instance()->sharedPaths()) {
        const QDir dir(folder);
        if (dir.exists())
            m_sharedFolders.append(dir);
    }
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

void LocalFileContentResolver::find(const QString &keyword, const QDir &rootFolder, QStringList *results)
{
    Q_ASSERT(results);

    QStringList nameFilters;
    nameFilters << FILE_TYPES[0].arg(keyword);
    nameFilters << FILE_TYPES[1].arg(keyword);

    QStringList entries = rootFolder.entryList(nameFilters,
                                               QDir::Files | QDir::AllDirs |
                                               QDir::NoDotAndDotDot | QDir::Readable, QDir::Name);

    for (const QString &name : entries) {
        const QFileInfo &info(rootFolder.absolutePath()+ QStringLiteral("/") + name);
        if (info.isDir()) {
            find(keyword, QDir(rootFolder.absolutePath()+ QStringLiteral("/") + name), results);
        } else {
            if (info.isFile()) {
                qDebug() << "Found: " << name;
                results->append(rootFolder.absolutePath()+ QStringLiteral("/") + name);
            }
        }
    }
}

QStringList LocalFileContentResolver::matches(const QString &keyword)
{
    QString searchKey = keyword;
    if (searchKey.contains("."))
        searchKey.truncate(searchKey.lastIndexOf('.'));
    searchKey += QChar('*');
    searchKey.replace(QStringLiteral("%20"), QStringLiteral(" "));
    searchKey.replace(QRegExp(QStringLiteral("%(.*)")), QStringLiteral("*"));

    QStringList results;
    for (const QDir &sharedDir : m_sharedFolders) {
        find(searchKey, sharedDir, &results);
    }
    return results;
}



