system(cd $$OUT_PWD && $$PWD/configure.sh)
OTHER_FILES += configure.sh

include(global.pri)

TEMPLATE = subdirs
CONFIG += ordered

SERVER_ONLY {
    DEFINES += SERVER_ONLY

    SUBDIRS +=  utils \
        libstreamphony \
        audioserver
}

!SERVER_ONLY {

SUBDIRS +=  utils \
    3rdparty \
    libstreamphony \
    audioserver \
    audioplayer \
    main/main.pro
}


