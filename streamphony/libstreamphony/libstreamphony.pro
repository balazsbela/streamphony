#-------------------------------------------------
#
# Project created by QtCreator 2014-05-03T13:56:07
#
#-------------------------------------------------

include(../global.pri);
include(../utils/utils.pri);

QT       -= gui

TARGET = streamphony
TEMPLATE = lib
CONFIG += staticlib bitdht

SOURCES += \
    dht/dhtmanager.cpp \
    dht/dhtcallbacks.cpp

HEADERS += \
    dht/dhtmanager.h \
    dht/dhtdebug.h \
    dht/dhtcallbacks.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

addModule(3rdparty/libbitdht, src/lib/libbitdht.a)

INCLUDEPATH += $$PWD/../3rdparty/libbitdht/src/
