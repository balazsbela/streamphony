#-------------------------------------------------
#
# Project created by QtCreator 2014-05-03T13:56:07
#
#-------------------------------------------------

include(../global.pri);
include(../utils/utils.pri);

QT += network xml gui phonon4qt5 quick

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
    audioplayer/mediastream.cpp \
    audioplayer/phononmediaplayer.cpp \
    xmpp/gui/rosterItem.cpp \
    xmpp/gui/rosterItemModel.cpp \
    xmpp/gui/rosterItemSortFilterProxyModel.cpp \
    xmpp/gui/xmppimageprovider.cpp \
    audioplayer/restclient.cpp

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
    audioplayer/mediastream.h \
    audioplayer/phononmediaplayer.h \
    xmpp/gui/rosterItem.h \
    xmpp/gui/rosterItemModel.h \
    xmpp/gui/rosterItemSortFilterProxyModel.h \
    xmpp/gui/xmppimageprovider.h \
    audioplayer/restclient.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

include(modules.pri);


