system(cd $$OUT_PWD && $$PWD/configure.sh)
OTHER_FILES += configure.sh

include(global.pri)

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS +=  utils \
    3rdparty \
    libstreamphony \
    mainapp \
    audioserver \
    audioplayer \    
    network


