#-------------------------------------------------
#
# Project created by QtCreator 2014-05-03T13:56:07
#
#-------------------------------------------------

include(../global.pri);
include(../utils/utils.pri);

QT       -= gui

QT += network xml

TARGET = streamphony
TEMPLATE = lib
CONFIG += staticlib bitdht

SOURCES += \
    dht/dhtmanager.cpp \
    dht/dhtcallbacks.cpp \
    xmpp/xmppmanager.cpp \
    settings/settingsmanager.cpp

HEADERS += \
    dht/dhtmanager.h \
    dht/dhtdebug.h \
    dht/dhtcallbacks.h \
    xmpp/xmppmanager.h \
    settings/settingsmanager.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

include(modules.pri);


