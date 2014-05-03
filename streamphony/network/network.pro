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

# Add libstreamphony, this ugly boilerplate is generated by qtcreator
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libstreamphony/release/ -llibstreamphony
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libstreamphony/debug/ -llibstreamphony
else:unix: LIBS += -L$$OUT_PWD/../libstreamphony/ -llibstreamphony

INCLUDEPATH += $$PWD/../libstreamphony
DEPENDPATH += $$PWD/../libstreamphony

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libstreamphony/release/liblibstreamphony.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libstreamphony/debug/liblibstreamphony.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libstreamphony/release/libstreamphony.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libstreamphony/debug/libstreamphony.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libstreamphony/liblibstreamphony.a
#----------------------------------------------------------------------------------
