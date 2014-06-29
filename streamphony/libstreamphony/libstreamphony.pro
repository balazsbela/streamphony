#-------------------------------------------------
#
# Project created by QtCreator 2014-05-03T13:56:07
#
#-------------------------------------------------

include(../global.pri);
include(../utils/utils.pri);

QT += network xml gui quick

!SERVER_ONLY {
    QT += phonon4qt5
}

TARGET = streamphony
TEMPLATE = lib
CONFIG += staticlib bitdht

SOURCES += \
    dht/dhtmanager.cpp \
    dht/dhtcallbacks.cpp \
    xmpp/xmppmanager.cpp \
    xmpp/vcardcache.cpp \
    xmpp/utils.cpp \
    settings/settingsmanager.cpp \
    daemon/lighthttpdaemon.cpp \
    daemon/localfilecontentresolver.cpp \
    daemon/connectionmanager.cpp \
    daemon/node.cpp \
    xmpp/gui/rosterItem.cpp \
    xmpp/gui/rosterItemModel.cpp \
    xmpp/gui/rosterItemSortFilterProxyModel.cpp \
    xmpp/gui/xmppimageprovider.cpp \
    daemon/gui/searchresultmodel.cpp \
    daemon/gui/searchresultitem.cpp \
    daemon/gui/listmodel.cpp

HEADERS += \
    dht/dhtmanager.h \
    dht/dhtdebug.h \
    dht/dhtcallbacks.h \
    xmpp/xmppmanager.h \
    xmpp/vcardcache.h \
    xmpp/utils.h \
    settings/settingsmanager.h \
    daemon/lighthttpdaemon.h \
    daemon/localfilecontentresolver.h \
    daemon/contentresolver.h \
    daemon/portrange.h \
    daemon/connectionmanager.h \
    daemon/node.h \
    daemon/nodestatus.h \
    dht/dhtconversions.h \
    xmpp/gui/rosterItem.h \
    xmpp/gui/rosterItemModel.h \
    xmpp/gui/rosterItemSortFilterProxyModel.h \
    xmpp/gui/xmppimageprovider.h \
    daemon/gui/searchresultmodel.h \
    daemon/gui/searchresultitem.h \
    daemon/gui/listmodel.h


!SERVER_ONLY {
    SOURCES += \
        audioplayer/mediastream.cpp \
        audioplayer/phononmediaplayer.cpp \
        audioplayer/restclient.cpp

    HEADERS = \
        audioplayer/mediastream.h \
        audioplayer/phononmediaplayer.h \
        audioplayer/restclient.h \
}

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

!SERVER_ONLY {
    include(modules.pri);
}


