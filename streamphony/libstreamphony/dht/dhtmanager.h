#ifndef DHTMANAGER_H
#define DHTMANAGER_H

#include <QObject>

class DhtManager : public QObject
{
    Q_OBJECT
public:
    explicit DhtManager(QObject *parent = 0);
    ~DhtManager() override;

signals:

public slots:

};

#endif // DHTMANAGER_H
