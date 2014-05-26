#ifndef BITDHT_MSGS_H
#define BITDHT_MSGS_H

/*
 * bitdht/bdmsgs.h
 *
 * BitDHT: An Flexible DHT library.
 *
 * Copyright 2010 by Robert Fernie
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 3 as published by the Free Software Foundation.
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


#include <stdio.h>
#include <inttypes.h>
#include <list>
#include "bitdht/bencode.h"
#include "bitdht/bdobj.h"
#include "bitdht/bdpeer.h"

#define BITDHT_MSG_TYPE_UNKNOWN         0
#define BITDHT_MSG_TYPE_PING            1
#define BITDHT_MSG_TYPE_PONG            2
#define BITDHT_MSG_TYPE_FIND_NODE       3
#define BITDHT_MSG_TYPE_REPLY_NODE      4
#define BITDHT_MSG_TYPE_GET_HASH        5
#define BITDHT_MSG_TYPE_REPLY_HASH      6
#define BITDHT_MSG_TYPE_REPLY_NEAR      7
#define BITDHT_MSG_TYPE_POST_HASH       8
#define BITDHT_MSG_TYPE_REPLY_POST      9
#define BITDHT_MSG_TYPE_REPLY_POST_HASH 10



// THESE ARE EXTENSIONS
#define BITDHT_MSG_TYPE_CONNECT     	20

// CONNECTIONS.
#define BITDHT_MSG_TYPE_CONNECT_REQUEST     	101
#define BITDHT_MSG_TYPE_CONNECT_REPLY     	102
#define BITDHT_MSG_TYPE_CONNECT_START     	103
#define BITDHT_MSG_TYPE_CONNECT_ACK     	104

// FANCY HASHES.


#define BITDHT_COMPACTNODEID_LEN 	26
#define BITDHT_COMPACTPEERID_LEN 	6

#define BE_Y_UNKNOWN    0
#define BE_Y_R          1
#define BE_Y_Q          2


/****** Known BD Version Strings ******/

#define BITDHT_VID_RS1	1
#define BITDHT_VID_UT	2


int bitdht_create_ping_msg(bdToken *tid, bdNodeId *id, char *msg, int avail);
int bitdht_response_ping_msg(bdToken *tid, bdNodeId *id, bdToken *vid, char *msg, int avail);
int bitdht_find_node_msg(bdToken *tid, bdNodeId *id, bdNodeId *target, char *msg, int avail);
int bitdht_resp_node_msg(bdToken *tid, bdNodeId *id, std::list<bdId> &nodes,
                                        char *msg, int avail);
// get_hash
int bitdht_get_peers_msg(bdToken *tid, bdNodeId *id, bdNodeId *info_hash,
                                        char *msg, int avail);
int bitdht_peers_reply_hash_msg(bdToken *tid, bdNodeId *id,
                                bdToken *token,
                                std::list<std::string> &values,
                                char *msg, int avail);

// post_hash
int bitdht_post_hash_msg(bdToken *transId,
                         bdNodeId *id, bdNodeId *key,
                         std::string hash, std::string secret,
                         char *const msg, int avail);

// bitdht_announce_peers_msg expects a
// bdToken *token
// This is because the last request made to the node
// which we will announce with returned a token. The next request to that node
// (bitdht_announce_peers_msg) must provide the same token or the request
// will be ignored. This is bittorrents way of ensuring that the
// value written to the key:value (value being ip:port tuples) is the
// remembered ip:port from the last requests source/sender, and it doesn't
// blindly write the provided value.
// For post_hash, we have just completed a find_node request which doesn't
//
// In our case, we're going to have to trust the peer. We don't care about
// writing an ip:port, so we don't care about remembering the souce of the last
// request.

int bitdht_reply_post_hash_msg(bdToken *tid, bdNodeId *id, bool successful,
                               char *msg, int avail);

int bitdht_peers_reply_closest_msg(bdToken *tid, bdNodeId *id,
                                bdToken *token, std::list<bdId> &nodes,
                                        char *msg, int avail);
int bitdht_announce_peers_msg(bdToken *tid, bdNodeId *id, bdNodeId *info_hash,
                              uint32_t port, bdToken *token, char *msg, int avail);
int bitdht_reply_announce_msg(bdToken *tid, bdNodeId *id,
                                        char *msg, int avail);


// Extensions.
int bitdht_connect_genmsg(bdToken *tid, bdNodeId *id, int msgtype, bdId *src, bdId *dest, int mode, int param, int status, char *msg, int avail);


//int response_peers_message()
//int response_closestnodes_message()

be_node *beMsgGetDictNode(be_node *node, const char *key);
int beMsgGetData(be_node *n, char *hash, uint &hashlen);
int beMsgMatchString(be_node *n, const char *str, int len);
uint32_t beMsgGetY(be_node *n);
uint32_t beMsgType(be_node *n);

bool bitdht_msgtype(uint32_t msg_type, std::string &name);


uint32_t convertBdVersionToVID(bdVersion *version);

be_node *makeCompactPeerIds(std::list<std::string> &values);
be_node *makeCompactNodeIdString(std::list<bdId> &nodes);

int beMsgGetToken(be_node *n, bdToken &token);
int beMsgGetNodeId(be_node *n, bdNodeId &nodeId);
int beMsgGetBdId(be_node *n, bdId &id);
int beMsgGetListBdIds(be_node *n, std::list<bdId> &nodes);

int beMsgGetListStrings(be_node *n, std::list<std::string> &values);
int beMsgGetUInt32(be_node *n, uint32_t *port);

/* Low Level conversion functions */
int decodeCompactPeerId(struct sockaddr_in *addr, char *enc, int len);
std::string encodeCompactPeerId(struct sockaddr_in *addr);

int decodeCompactNodeId(bdId *id, char *enc, int len);
std::string encodeCompactNodeId(bdId *id);



#endif
