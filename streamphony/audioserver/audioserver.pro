#-------------------------------------------------
#
# Project created by QtCreator 2014-04-06T17:54:33
#
#-------------------------------------------------

include(../global.pri);
include(../utils/utils.pri);

QT       += core network

!SERVER_ONLY {
    QT += phonon4qt5
}

QT       -= gui

TARGET = audioserver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

addModule(libstreamphony, libstreamphony.a)
include(../libstreamphony/modules.pri);
