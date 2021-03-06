#-------------------------------------------------
#
# Project created by QtCreator 2014-04-14T20:02:10
#
#-------------------------------------------------

include(../global.pri);
include(../utils/utils.pri);

QT += core network xml gui phonon4qt5 quick qml widgets

TARGET = main
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

addModule(libstreamphony, libstreamphony.a)
include(../libstreamphony/modules.pri);

OTHER_FILES += bdboot.txt \
    gui/Main.qml \
    gui/PlaybackScreen.qml \
    gui/ProgressBar.qml \
    gui/FriendList.qml \
    gui/Results.qml \
    gui/ScrollBar.qml

peerlist.path = $$OUT_PWD
peerlist.files = $$PWD/bdboot.txt
INSTALLS += peerlist

RESOURCES += \
    resources.qrc


