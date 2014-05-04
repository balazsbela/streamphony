# global settings for all subdirs
# should be included in all .pro files

QMAKE_CXXFLAGS += -Wall -Wextra -Wnon-virtual-dtor -Woverloaded-virtual
QMAKE_CXXFLAGS_RELEASE += -Os
QMAKE_CXXFLAGS_DEBUG += -g3
QMAKE_LFLAGS += -Wl,--fatal-warnings -Wl,--no-undefined -lc
CONFIG += qml_debug declarative_debug
CONFIG += c++11

defineTest(addModule) {
    module_path = $$1
    library_name = $$2
    extra_libs = $$3

    isEmpty(library_name) : library_name = lib$$basename(module_path).a
    library_path = $${BUILD_ROOT}/$${module_path}/$${library_name}

    !exists($${SOURCE_ROOT}/$${module_path}) : error("Cannot find module $${module_path}")

    INCLUDEPATH *= \
        $${SOURCE_ROOT}/$${module_path} \
        $${BUILD_ROOT}/$${module_path}

    DEPENDPATH *= \
        $${SOURCE_ROOT}/$${module_path} \
        $${BUILD_ROOT}/$${module_path}

    export(INCLUDEPATH)
    export(DEPENDPATH)

    !equals(TEMPLATE, lib) {
        LIBS -= $${library_path}
        LIBS += $${library_path} $${extra_libs}

        PRE_TARGETDEPS *= $${library_path}

        export(LIBS)
        export(PRE_TARGETDEPS)
    }

    return(true)
}
