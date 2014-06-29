/*
 * Copyright (C) 2008-2014 The QXmpp developers
 *
 * Author:
 *	Manjeet Dahiya
 *
 * Source:
 *	https://github.com/qxmpp-project/qxmpp
 *
 * This file is a part of QXmpp library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 */


#include "rosterItemModel.h"

RosterItemModel::RosterItemModel(QObject* parent) : QStandardItemModel(parent)
{
}

rosterItem* RosterItemModel::getRosterItemFromBareJid(const QString& bareJid)
{
    if(m_jidRosterItemMap.contains(bareJid))
        return m_jidRosterItemMap[bareJid];
    else
        return 0;
}

rosterItem* RosterItemModel::getOrCreateItem(const QString& bareJid)
{
    if(m_jidRosterItemMap.contains(bareJid)) {
        return m_jidRosterItemMap[bareJid];
    } else {
        rosterItem* item = new rosterItem(bareJid);
        m_jidRosterItemMap[bareJid] = item;
        appendRow(item);
        return item;
    }
}

void RosterItemModel::updatePresence(const QString& bareJid, const QMap<QString, QXmppPresence>& presences)
{
    rosterItem *item = getOrCreateItem(bareJid);
    if (!presences.isEmpty()) {
        item->setPresence(*presences.begin());
    } else {
        item->setPresence(QXmppPresence(QXmppPresence::Unavailable));
        removeRosterEntry(bareJid);
    }
}

void RosterItemModel::updateRosterEntry(const QString& bareJid, const QXmppRosterIq::Item& rosterEntry)
{
    getOrCreateItem(bareJid)->setName(rosterEntry.name());
}

void RosterItemModel::updateAvatar(const QString& bareJid, const QImage& image)
{
    getOrCreateItem(bareJid)->setAvatar(image);
}

void RosterItemModel::updateName(const QString& bareJid, const QString& name)
{
    if (!name.isEmpty())
        getOrCreateItem(bareJid)->setName(name);
}

void RosterItemModel::clear()
{
    QStandardItemModel::clear();
    m_jidRosterItemMap.clear();
}

void RosterItemModel::removeRosterEntry(const QString& bareJid)
{
    rosterItem* item = getRosterItemFromBareJid(bareJid);
    if(item)
    {
        removeRow(item->row());
    }
}

QVariant RosterItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_jidRosterItemMap.keys().size())
        return QVariant();

    const rosterItem *item = m_jidRosterItemMap[m_jidRosterItemMap.keys()[index.row()]];

    switch(role) {
        case NameRole:
          return item->getName();
        case AvatarRole:
          return item->getAvatar();
        case BareJidRole:
          return item->getJid();
    }

    return QVariant();
}

int RosterItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_jidRosterItemMap.keys().size();
}

QHash<int, QByteArray> RosterItemModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(NameRole, "name");
    roles.insert(AvatarRole, "avatar");
    roles.insert(BareJidRole, "bareJid");
    return roles;
}
