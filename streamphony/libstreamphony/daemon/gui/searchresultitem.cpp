#include "searchresultitem.h"

SearchResultItem::SearchResultItem(const QString &fileName, const QString &ownerJid) :
    QStandardItem(),
    m_fileName(fileName),
    m_ownerJid(ownerJid)
{
    setData(m_fileName, SearchResultItem::FileName);
    setData(m_ownerJid, SearchResultItem::OwnerJid);
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
