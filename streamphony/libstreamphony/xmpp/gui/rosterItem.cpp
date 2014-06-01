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


#include "rosterItem.h"
#include <QImage>

rosterItem::rosterItem(const QString& bareJid)
{
    setData(bareJid, rosterItem::BareJid);
    m_jid = bareJid;

    setData("Offline", rosterItem::StatusText);
    setAvatar(QImage(":/icons/resource/avatar.png"));
    setIcon(QIcon(":/icons/resource/gray.png"));
}

void rosterItem::setName(const QString& name)
{
    setText(name);
}

QString rosterItem::getName() const
{
    return text();
}

void rosterItem::setPresence(const QXmppPresence &presence)
{
    // determine status text
    QString statusText = presence.statusText();
    if (statusText.isEmpty()) {
        if(presence.type() == QXmppPresence::Available)
            statusText = "Available";
        else if(presence.type() == QXmppPresence::Unavailable)
            statusText = "Offline";
    }

    // store data
    setData(statusText, rosterItem::StatusText);
    setData(static_cast<int>(presence.type()), PresenceType);
    setData(static_cast<int>(presence.availableStatusType()), StatusType);

    // update icon
    QString icon;
    if (presence.type() == QXmppPresence::Available) {
        switch (presence.availableStatusType())
        {
        case QXmppPresence::Online:
        case QXmppPresence::Chat:
            icon = "green";
            break;
        case QXmppPresence::Away:
        case QXmppPresence::XA:
            icon = "orange";
            break;
        case QXmppPresence::DND:
            icon = "red";
            break;
        case QXmppPresence::Invisible:
            icon = "gray";
            break;
        }
    } else {
        icon = "gray";
    }
    if (!icon.isEmpty())
        setIcon(QIcon(":/icons/resource/"+icon+".png"));
}

void rosterItem::setAvatar(const QImage& image)
{
    setData(QVariant(image), rosterItem::Avatar);
}

QImage rosterItem::getAvatar() const
{
    return qvariant_cast<QImage>(data(rosterItem::Avatar));
}

QString rosterItem::getJid() const
{
    return m_jid;
}
