#-------------------------------------------------
#
# Project created by QtCreator 2014-04-14T20:02:10
#
#-------------------------------------------------

include(../global.pri);
include(../utils/utils.pri);

QT += core network xml gui

TARGET = network
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

addModule(libstreamphony, libstreamphony.a)
include(../libstreamphony/modules.pri);

OTHER_FILES += bdboot.txt

peerlist.path = $$OUT_PWD
peerlist.files = $$PWD/bdboot.txt
INSTALLS += peerlist

