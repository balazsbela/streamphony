#ifndef NODESTATUS_H
#define NODESTATUS_H

#include <QString>
#include <QObject>

class NodeStatus : public QObject
{
    Q_OBJECT           
public:

    NodeStatus(QObject *parent = 0) : QObject(parent) {

    }

    explicit NodeStatus(const QString &jid, bool queryRunning, QObject *parent = 0) :
        QObject(parent),
        m_jid(jid),
        m_queryRunning(queryRunning)
    {
    }

    NodeStatus(const NodeStatus &other) {
        m_jid = other.jid();
        m_queryRunning = other.queryRunning();
    }

    ~NodeStatus() override {

    }

    bool queryRunning() const {
        return m_queryRunning;
    }

    void setQueryRunning(bool queryRunning) {
        m_queryRunning = queryRunning;
    }

    QString jid() const {
        return m_jid;
    }

    NodeStatus& operator= (const NodeStatus &other) {
        this->m_jid = other.m_jid;
        this->m_queryRunning = other.m_queryRunning;
        return *this;
    }

private:
    bool m_queryRunning = false;
    QString m_jid;
};

#endif // NODESTATUS_H
