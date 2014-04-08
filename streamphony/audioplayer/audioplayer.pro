#-------------------------------------------------
#
# Project created by QtCreator 2014-04-06T22:19:56
#
#-------------------------------------------------

include(../global.pri);
include(../utils/utils.pri);

QT       += core multimedia phonon4qt5

QT       -= gui

TARGET = audioplayer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    phononmediaplayer.cpp

HEADERS += \
    phononmediaplayer.h

DEFINES += DEBUG_AUDIO_PLAYER
