# global settings for all subdirs
# should be included in all .pro files

QMAKE_CXXFLAGS += -Wall -Wextra -Wnon-virtual-dtor -Woverloaded-virtual
QMAKE_CXXFLAGS_RELEASE += -Os
QMAKE_CXXFLAGS_DEBUG += -g3
QMAKE_LFLAGS += -Wl,--fatal-warnings -Wl,--no-undefined -lc
CONFIG += qml_debug declarative_debug
CONFIG += c++11

defineTest(addStaticLibrary) {
    LIBS += $$1
    export(LIBS)

    PRE_TARGETDEPS += $$1
    export(PRE_TARGETDEPS)

    return(true)
}
