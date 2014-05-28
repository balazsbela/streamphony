#ifndef DHTCONVERSIONS_H
#define DHTCONVERSIONS_H

#include "bitdht/bdiface.h"

#include <QString>
#include <QByteArray>

// Conversion utilities to make the library usable.

namespace DhtConversions {

bdId toBdId(const dhtNode &node)
{
    bdNodeId nodeId;
    memcpy(nodeId.data, node.id.data(), BITDHT_KEY_LEN);
    return bdId(nodeId, node.addr);
}

QByteArray toByteArray(const bdId *id)
{
    char* idArray = new char[BITDHT_KEY_LEN + 1];
    memcpy(idArray, id->id.data, BITDHT_KEY_LEN);
    idArray[BITDHT_KEY_LEN+1] = '\0';
    return QByteArray(idArray, BITDHT_KEY_LEN);
}

QByteArray toByteArray(const bdNodeId *id)
{
    char* idArray = new char[BITDHT_KEY_LEN + 1];
    memcpy(idArray, id->data, BITDHT_KEY_LEN);
    idArray[BITDHT_KEY_LEN+1] = '\0';
    return QByteArray(idArray, BITDHT_KEY_LEN);
}

bdNodeId fromByteArray(const QByteArray &hash)
{
    bdNodeId result;
    memcpy(result.data, hash.data(), BITDHT_KEY_LEN);
    return result;
}

QString bdIdToString(const bdId *id)
{
    return toByteArray(id).toHex();
}

QString bdNodeIdToString(const bdNodeId *id)
{
    return toByteArray(id).toHex();
}

} // namespace

#endif // DHTCONVERSIONS_H
