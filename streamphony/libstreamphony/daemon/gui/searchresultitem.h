#ifndef SEARCHRESULTITEM_H
#define SEARCHRESULTITEM_H

#include "listmodel.h"

#include <QObject>
#include <QStandardItem>

class SearchResultItem : public ListItem
{
    Q_OBJECT
public:
    SearchResultItem(QObject *parent = 0);
    SearchResultItem(const SearchResultItem &other);
    explicit SearchResultItem(const QString &fileName, const QString &ownerJid, QObject *parent = 0);
    ~SearchResultItem() override;

    SearchResultItem& operator=(const SearchResultItem &other);

    enum userRoles
    {
        FileName = Qt::UserRole + 2,
        OwnerJid
    };

    QString id() const override;
    QVariant data(int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString fileName() const;
    QString ownerJid() const;

private:
    QString m_fileName;
    QString m_ownerJid;
};

#endif // SEARCHRESULTITEM_H
