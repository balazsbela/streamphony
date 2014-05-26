/*
 * libretroshare/src/dht: bdhandler.h
 *
 * BitDht example interface
 *
 * Copyright 2009-2010 by Robert Fernie.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "bitdht@lunamutt.com".
 *
 */


// #include <udp/udpstack.h>
#include <udp/udpbitdht.h>
// #include <bitdht/bdstddht.h>
#include <cstring>

#include "bdHandler.h"


// class BdCallback: public BitDhtCallback
// {
// public:
//     BdCallback(BitDhtHandler *parent)
//      :mParent(parent)
//     {
//         return;
//     }

//     virtual int dhtPeerCallback(const bdId *id, uint32_t status)
//     {
//         return mParent->PeerCallback(id, status);
//     }

// private:
//     BitDhtHandler *mParent;
// };

BitDhtHandler::BitDhtHandler()
: m_gotHashResult(false),
  m_getHashValue(),
  m_postHashGotResult(false),
  m_postHashSuccess(false)
{
}
BitDhtHandler::~BitDhtHandler()
{
    ;
}

void BitDhtHandler::clearResult()
{
    m_gotHashResult = false;
    m_getHashValue.clear();
    m_postHashGotResult = false;
    m_postHashSuccess = false;
}

bool BitDhtHandler::FindNode(UdpBitDht *udpBitDht, bdNodeId *peerId)
{
    std::cerr << "BitDhtHandler::FindNode(";
    bdStdPrintNodeId(std::cerr, peerId);
    std::cerr << ")" << std::endl;

    BssResult res;
    res.id.id = *peerId;
    res.mode = BITDHT_QFLAGS_DISGUISE;
    // res.mode = BITDHT_QFLAGS_NONE;

    res.status = 0;
    {
        bdStackMutex stack(resultsMtx);
        mSearchNodes[*peerId] = res;
    }

    udpBitDht->addFindNode(peerId, BITDHT_QFLAGS_DISGUISE);

    return true ;
}

bool BitDhtHandler::GetHash(UdpBitDht *udpBitDht,
                            bdId *targetNode,
                            bdNodeId *key)
{
    std::cerr << "BitDhtHandler::getHash(";
    bdStdPrintId(std::cerr, targetNode);
    std::cerr << ", key: " << key << ")" << std::endl;

    BssResult res;
    res.id.id = targetNode->id;
    res.mode = BITDHT_QFLAGS_DISGUISE;
    res.status = 0;

    // add results vars
    // bdStackMutex stack(resultsMtx);
    // virtual void getHash(bdId &id, bdNodeId &key);
    udpBitDht->getHash(*targetNode, *key);

    return true ;
}

bool BitDhtHandler::SearchResult(bdId *id, uint32_t &status)
{
    bdStackMutex stack(resultsMtx);

    /* find the node from our list */
    std::map<bdNodeId, BssResult>::iterator it;
    it = mSearchNodes.find(id->id);
    if (it != mSearchNodes.end())
    {
        if (it->second.status != 0)
        {
            std::cerr << "BitDhtHandler::SearchResults() Found Results";
            std::cerr << std::endl;
            status = it->second.status;
            *id = it->second.id;
            return true;
        }

        std::cerr << "BitDhtHandler::SearchResults() No Results Yet";
        std::cerr << std::endl;
        return false;
    }

    std::cerr << "BitDhtHandler::SearchResults() ERROR: No Search Entry";
    std::cerr << std::endl;
    return false;
}


int BitDhtHandler::dhtPeerCallback(const bdId *id, uint32_t status)
{
    std::cerr << "BitDhtHandler::PeerCallback() NodeId: ";
    bdStdPrintId(std::cerr, id);
    std::cerr << std::endl;

    bdStackMutex stack(resultsMtx); /********** MUTEX LOCKED *************/

    /* find the node from our list */
    std::map<bdNodeId, BssResult>::iterator it;
    it = mSearchNodes.find(id->id);
    if (it == mSearchNodes.end())
    {
        std::cerr << "BitDhtHandler::PeerCallback() Unknown NodeId !!! ";
        std::cerr << std::endl;

        return 1;
    }
    it->second.status = status;

    switch(status)
    {
        case BITDHT_MGR_QUERY_FAILURE:
            /* do nothing */
            std::cerr << "BitDhtHandler::PeerCallback() QUERY FAILURE ... do nothin ";
            std::cerr << std::endl;
        break;

        case BITDHT_MGR_QUERY_PEER_OFFLINE:
            /* do nothing */
            std::cerr << "BitDhtHandler::PeerCallback() QUERY PEER OFFLINE ... do nothin ";
            std::cerr << std::endl;
            break;

        case BITDHT_MGR_QUERY_PEER_UNREACHABLE:
            /* do nothing */
            std::cerr << "BitDhtHandler::PeerCallback() QUERY PEER UNREACHABLE ... saving address ";
            std::cerr << std::endl;
            it->second.id = *id;
        break;

        case BITDHT_MGR_QUERY_PEER_ONLINE:
            /* do something */
            std::cerr << "BitDhtHandler::PeerCallback() QUERY PEER ONLINE ... saving address";
            std::cerr << std::endl;
            it->second.id = *id;
        break;
    }
    return 1;
}

int BitDhtHandler::dhtValueCallback(const bdId *id, std::string hash, uint32_t status)
{
    std::cerr << "BitDhtHandler::PeerCallback() NodeId: ";
    bdStdPrintId(std::cerr, id);
    std::cerr << std::endl;

    bdStackMutex stack(resultsMtx); /********** MUTEX LOCKED *************/

    // of course, this will break if the hash returned from a get_hash request
    // contains the string post_hash_success. A better way to do this would
    // be to have another QUERY_TYPE param passed to this fn which we could
    // switch on.
    if(strcmp(hash.data(), "post_hash_success") == 0)
    {
        std::cerr << "post_hash completed." << hash << std::endl;
        m_postHashGotResult = true;
        m_postHashSuccess = true;
    }
    else
    {
        std::cerr << "get_hash completed." << hash << std::endl;
        m_getHashValue = hash;
        m_gotHashResult = true;
    }

    return 1;
}
