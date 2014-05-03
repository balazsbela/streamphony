#-------------------------------------------------
#
# Project created by QtCreator 2014-05-03T13:56:07
#
#-------------------------------------------------

include(../global.pri);
include(../utils/utils.pri);

QT       -= gui

TARGET = libstreamphony
TEMPLATE = lib
CONFIG += staticlib bitdht

SOURCES += \
    dht/dhtmanager.cpp

HEADERS += \
    dht/dhtmanager.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../3rdparty/libbitdht/src/lib/release/ -lbitdht
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../3rdparty/libbitdht/src/lib/debug/ -lbitdht
else:unix: LIBS += -L$$OUT_PWD/../3rdparty/libbitdht/src/lib -lbitdht

INCLUDEPATH += $$PWD/../3rdparty/libbitdht/src
DEPENDPATH += $$PWD/../3rdparty/libbitdht/src/lib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/libbitdht/src/lib/release/libbitdht.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/libbitdht/src/lib/debug/libbitdht.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/libbitdht/src/lib/release/bitdht.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/libbitdht/src/lib/debug/bitdht.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/libbitdht/src/lib/libbitdht.a
