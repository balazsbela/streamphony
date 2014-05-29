#-------------------------------------------------
#
# Project created by QtCreator 2014-05-03T13:56:07
#
#-------------------------------------------------

include(../global.pri);
include(../utils/utils.pri);

QT += network xml gui phonon4qt5

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
    audioplayer/phononmediaplayer.cpp

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
    audioplayer/phononmediaplayer.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

include(modules.pri);


