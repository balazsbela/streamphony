#-------------------------------------------------
#
# Project created by QtCreator 2014-04-14T20:02:10
#
#-------------------------------------------------

include(../global.pri);

QT       += core network

QT       -= gui

TARGET = network
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

addModule(libstreamphony, libstreamphony.a)
addModule(3rdparty/libbitdht, src/lib/libbitdht.a)

INCLUDEPATH += $$PWD/../3rdparty/libbitdht/src/

