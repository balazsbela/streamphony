/*
 * libbitdht/src/auth/bdHandler.h
 *
 * BitDht interface example
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

#ifndef BITDHT_AUTH_TEST_HANDLER_H
#define BITDHT_AUTH_TEST_HANDLER_H

#include <string>
#include <map>

// #include "udp/udpstack.h"
// #include "udp/udpbitdht.h"
// #include "bitdht/bdiface.h"

class UdpBitDht;

class BssResult
{
    public:
    bdId id;
    uint32_t mode;      // single shot
    uint32_t status;    // SEARCHING, FAILURE, FOUND, MULTIPLE HITS.
};


class BitDhtHandler: public BitDhtCallback
{
public:
    BitDhtHandler();
    ~BitDhtHandler();

    void clearResult();

    // start the find_node request
    bool FindNode(UdpBitDht *udpBitDht, bdNodeId *peerId);
    bool GetHash(UdpBitDht *udpBitDht, bdId *targetNode, bdNodeId *key);

    // callbacks for find_node response
    virtual int dhtPeerCallback(const bdId *id, uint32_t status);
    virtual int dhtValueCallback(const bdId *id, std::string key, uint32_t status);

    // poll this function to check if we have a result
    bool SearchResult(bdId *id, uint32_t &status);


    // must implement these to satify abstract base class
    virtual int dhtNodeCallback(const bdId *id, uint32_t peerflags)
    {
        return 1;
    }
    virtual int dhtConnectCallback(const bdId*, const bdId*, const bdId*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)
    {
        return 1;
    }
    virtual int dhtInfoCallback(const bdId*, uint32_t, uint32_t, std::string)
    {
        return 1;
    }

    // get_hash
    // TODO: proper implementation should have this as a map, with the key
    //       being the bdNodeId that the request was made to.
    //       In this case we know the test code is only making a single
    //       get_hash request.
    // Have we received a result?
    bool m_gotHashResult;
    // The returned hash value.
    std::string m_getHashValue;

    // post_hash
    // Have we received a result?
    bool m_postHashGotResult;
    // Was the hash posted successfully?
    bool m_postHashSuccess;

private:
    // find_node
    bdMutex resultsMtx; /* for all class data (below) */
    std::map<bdNodeId, BssResult> mSearchNodes;
};

#endif // BITDHT_AUTH_TEST_HANDLER_H
