
#Add 3rd party modules, easier to import in other subprojects

addModule(3rdparty/libbitdht, src/lib/libbitdht.a)
INCLUDEPATH += $$PWD/../3rdparty/libbitdht/src/

CONFIG(debug, debug|release) {
    addModule(3rdparty/qxmpp, build/lib/libqxmpp_d.a);
} else {
    addModule(3rdparty/qxmpp, build/lib/libqxmpp.a);
}

INCLUDEPATH += $$OUT_PWD/../3rdparty/qxmpp/build/include/qxmpp
