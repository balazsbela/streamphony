
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = bdexample

HEADERS += bdhandler.h

SOURCES += \
    main.cpp \
    bdhandler.cpp

OTHER_FILES += bdboot.txt

QMAKE_LFLAGS += -pthreads

peerlist.path = $$OUT_PWD
peerlist.files = $$PWD/bdboot.txt
INSTALLS += peerlist

DEFINES += DEBUG_BITDHT

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
