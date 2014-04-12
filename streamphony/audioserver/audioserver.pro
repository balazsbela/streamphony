#-------------------------------------------------
#
# Project created by QtCreator 2014-04-06T17:54:33
#
#-------------------------------------------------

include(../global.pri);
include(../utils/utils.pri);

QT       += core network

QT       -= gui

TARGET = audioserver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    lighthttpdaemon.cpp \
    localfilecontentresolver.cpp

HEADERS += \
    lighthttpdaemon.h \
    contentresolver.h \
    localfilecontentresolver.h
