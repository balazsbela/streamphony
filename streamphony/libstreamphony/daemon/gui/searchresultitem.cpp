#include "searchresultitem.h"

SearchResultItem::SearchResultItem(const QString &fileName, const QString &ownerJid, QObject *parent) :
    ListItem(parent),
    m_fileName(fileName),
    m_ownerJid(ownerJid)
{
}

SearchResultItem::SearchResultItem(QObject *parent) : ListItem(parent)
{
}

SearchResultItem::SearchResultItem(const SearchResultItem &other)
{
    m_fileName = other.fileName();
    m_ownerJid = other.ownerJid();
}

SearchResultItem::~SearchResultItem()
{
}

QString SearchResultItem::fileName() const
{
    return m_fileName;
}

QString SearchResultItem::ownerJid() const
{
    return m_ownerJid;
}

QVariant SearchResultItem::data(int role) const
{
    switch(role) {
    case FileName:
      return fileName();
    case OwnerJid:
      return ownerJid();
    default:
      return QVariant();
    }
}

QHash<int, QByteArray> SearchResultItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[FileName] = "filename";
    names[OwnerJid] = "jid";
    return names;
}

QString SearchResultItem::id() const
{
    return m_fileName;
}

SearchResultItem& SearchResultItem::operator=(const SearchResultItem &other)
{
    if (&other == this) {
        return *this;
    }

    m_fileName = other.fileName();
    m_ownerJid = other.ownerJid();

    return *this;
}
