#ifndef DHTDEBUG_H
#define DHTDEBUG_H

#include <QDebug>

#define DEBUG_DHT

#ifdef DEBUG_DHT
    #define debugDht qDebug
#else
    #define debugDht QT_NO_QDEBUG_MACRO
#endif

#endif // DHTDEBUG_H
