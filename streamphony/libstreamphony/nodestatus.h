#ifndef NODESTATUS_H
#define NODESTATUS_H

#include <QString>
#include <QObject>

class NodeStatus : public QObject
{
    Q_OBJECT           
public:
    explicit NodeStatus(const QString &jid, QObject *parent = 0) :
        QObject(parent),
        m_jid(jid)
    {
    }

    bool queryRunning() const {
        return m_queryRunning;
    }

    void setQueryRunning(bool queryRunning) {
        m_queryRunning = queryRunning;
    }

private:
    bool m_queryRunning = false;
    QString m_jid;
};

#endif // NODESTATUS_H
