#ifndef SEARCHRESULTMODEL_H
#define SEARCHRESULTMODEL_H

#include "searchresultitem.h"

#include <QAbstractListModel>

class SearchResultModel : public QAbstractListModel
{
    Q_OBJECT
public:
    SearchResultModel(QObject *parent = 0);
    ~SearchResultModel() override;

    enum Role {
        FileNameRole = Qt::UserRole + 1,
        JidRole
    };

    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addItems(const SearchResultItem &item);

private:
    QList<SearchResultItem> m_results;
};

#endif // SEARCHRESULTMODEL_H
