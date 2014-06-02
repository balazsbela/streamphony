#ifndef SEARCHRESULTITEM_H
#define SEARCHRESULTITEM_H

#include <QObject>
#include <QStandardItem>

class SearchResultItem : public QStandardItem
{
public:
    explicit SearchResultItem(const QString &fileName, const QString &ownerJid);
    ~SearchResultItem() override;

    enum userRoles
    {
        FileName = Qt::UserRole + 2,
        OwnerJid
    };

    QString fileName() const;
    QString ownerJid() const;

private:
    QString m_fileName;
    QString m_ownerJid;
};

#endif // SEARCHRESULTITEM_H
