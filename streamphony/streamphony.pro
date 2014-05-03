system(cd $$OUT_PWD && $$PWD/configure.sh)
OTHER_FILES += configure.sh

include(global.pri)

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS +=  utils \
    mainapp \
    audioserver \
    audioplayer \
    libstreamphony \
    network \
    bitdhtexample \
    3rdparty


