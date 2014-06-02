#include "searchresultmodel.h"

SearchResultModel::SearchResultModel(QObject *parent) : QAbstractListModel(parent)
{
}

SearchResultModel::~SearchResultModel()
{
}

QVariant SearchResultModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_results.size())
        return QVariant();

    return QStringLiteral("test");

    const SearchResultItem &item = m_results[index.row()];

    switch(role) {
        case FileNameRole:
          return item.fileName();
        case JidRole:
          return item.ownerJid();
    }

    return QVariant();
}

int SearchResultModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_results.size();
}

QHash<int, QByteArray> SearchResultModel::roleNames() const
{
   return {{FileNameRole, "filename"}, {JidRole, "jid"}};
}

void SearchResultModel::addItems(const SearchResultItem &item)
{
    m_results.append(item);
}
