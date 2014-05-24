TEMPLATE = lib
TARGET = portfwd_dummy
CONFIG += no_link staticlib

SOURCES =
HEADERS =

QMAKE_POST_LINK += rm libportfwd_dummy.a; sh $$PWD/build.sh $$PWD $$OUT_PWD;
