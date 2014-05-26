## init
^^^^^^^

// retroshare-gui
    // main.cpp
        // rsinit.cc
            RsControl::instance()->StartupRetroShare())
            // this is adding things to rsUdpStack *mDhtStack:
            UdpStunner mDhtStunner

            p3BitDht *mBitDht()
                // create p3BitDht *mBitDht
                // libretroshare/src/dht/p3bitdht.cc
                    bdModDht::bdModDht() at bdstddht.cc:334 0xd292fc
                    // libbitdht/src/bitdht/bdstddht.cc

                    mUdpBitDht = new UdpBitDht(udpstack, &mOwnDhtId, dhtVersion, bootstrapfile, mDhtFns);
                        // /libbitdht/src/udp/udpbitdht.h
                        /*
                         * This implements a UdpSubReceiver class to allow the DHT to talk to the network.
                         * The parser is very strict - and will try to not pick up anyone else's messages.
                         *
                         * Mutexes are implemented at this level protecting the whole of the DHT code.
                         * This class is also a thread - enabling it to do callback etc.
                         */
                        // class BitDhtCallback defined in bdiface.h

                        mBitDhtManager = new bdNodeManager(id, usedVersion, bootstrapfile, fns);
                            // libbitdht/src/bitdht/bdmanager.cc
                            /******************************************
                             * 1) Maintains a list of ids to search for.
                             * 2) Sets up initial search for own node.
                             * 3) Checks on status of queries.
                             * 4) Callback on successful searches.
                             *
                             * This is pretty specific to RS requirements.
                             ****/
                            virtual void addFindNode(bdNodeId *id, uint32_t mode);
                            virtual void removeFindNode(bdNodeId *id);
                            virtual void findDhtValue(bdNodeId *id, std::string key, uint32_t mode);

                            bdNode(id, dhtVersion, bootfile, fns) #
                            mBloomFilter (what is this for?) set in here




            UdpRelayReceiver *mRelay
            // LAST ON THIS STACK IS STANDARD DIRECT TOU
            udpReceivers[RSUDP_TOU_RECVER_DIRECT_IDX] = new UdpPeerReceiver


## RECEIVING DHT MESSAGES
^^^^^^^^^^^^^^^^^^^^^^^^^

startup kicks off a separate thread for DHT in rsinit.cc, and probably one for udplayer.
this thread
void UdpBitDht::run()
libbitdht/src/bitdht/bdmanager.cc - bdNode::iteration();
// this probably grabs dht messages frmo the udp layer messaging container / list
libbitdht/src/bitdht/bdnote.cc - recvPkt(msg->data, msg->mSize, msg->addr);


## CALLBACKS
^^^^^^^^^^^^

#### ADD A CALLBACK
// libretroshare/src/dht/p3bitdht.cc
// defines:
class p3BdCallback: public BitDhtCallback
// then in the constructor:
p3BitDht::p3BitDht()
    // 'this' being the current instance of p3BitDht
    // is stored in p3BdCallback.mParent
    p3BdCallback *bdcb = new p3BdCallback(this);
    mUdpBitDht->addCallback(bdcb);
        // this calls udpbitdht.addCallback(), which in turn calls:
        // libbitdht/src/bitdht/bdmanager.cc
        void bdNodeManager::addCallback(BitDhtCallback *cb)
            // cd (the instance of p3BitDht) is then added to the callback list
            // in bdmanager.cc:
            bdNodeManager.mCallbacks

            // bdNodeManager implements BitDhtInterface, defined in:
            // libbitdht/src/bitdht/bdiface.h
            BitDhtInterface.addCallback(BitDhtCallback *cb)


#### CALLBACK INVOKATION
     WHERE DOES bdNodeManager::status() GET CALLED?
// libbitdht/src/bitdht/bdmanager.cc
int bdNodeManager::status()
    checkStatus()
    int bdNodeManager::checkStatus()
        doPeerCallback(&id, callbackStatus);
        void bdNodeManager::doPeerCallback(const bdId *id, uint32_t status)
            // which calls to each BitDhtInterface stored instance in the mCallbacks
            // array:
            (*it)->dhtPeerCallback(id, status);
            // this goes to the class interface BitDhtCallback
            p3BdCallback: public BitDhtCallback
                virtual int dhtPeerCallback(const bdId *id, uint32_t status)
                    return mParent->PeerCallback(id, status);
                    // which calls the mParent instance, in this case is a p3BitDht
                    // libretroshare/src/dht/p3bitdht_peernet.cc
                    int p3BitDht::PeerCallback(const bdId *id, uint32_t status)
                        OnlinePeerCallback_locked(id, status, dpd);
                        // libretroshare/src/dht/p3bitdht_peernet.cc
                        int p3BitDht::OnlinePeerCallback_locked(const bdId *id, uint32_t /*status*/, DhtPeerDetails *dpd)
                            PeerAction ca;
                            mActions.push_back(ca);


#### MESSAGE RECV HANDLING & CALLBACK STACK
p3BdCallback::dhtNodeCallback() at p3bitdht.cc:54 0xac70f3
bdNodeManager::doNodeCallback() at bdmanager.cc:1,147 0xd3e5b7
bdNodeManager::addPeer() at bdmanager.cc:1,124 0xd3e5eb
bdNode::recvPkt() at bdnode.cc:1,555 0xd3ccf8
bdNode::iteration() at bdnode.cc:312 0xd3d1da
bdNodeManager::iteration() at bdmanager.cc:495 0xd3f4ff
UdpBitDht::run() at udpbitdht.cc:327 0xd2cdc7
bdthread_init() at bdthreads.cc:57 0xd44dbf

#### THIS STACK IS ALSO CALLED, TO CHECK QUERY STATUS (?)
bdNodeManager::checkStatus() at bdmanager.cc:696 0xd3f267
bdNodeManager::status() at bdmanager.cc:657 0xd3f3d3
bdNodeManager::iteration() at bdmanager.cc:443 0xd3f4af
UdpBitDht::run() at udpbitdht.cc:327 0xd2cdc7
bdthread_init() at bdthreads.cc:57 0xd44dbf


## MESSAGES
^^^^^^^^^^^

bdNode.cc has message handling for incoming and outgoing messages
bdManager.cc is a NodeManager which is itself a bdNode
bdmsgs has message creation and parsing functions


-? what are the different messages in bdmsgs.h, and how do they correspond to bittorrent messages.
  Some appear to not be called from anywhere else, like msgout_get_hash and msgout_reply_hash. Surely RS has to reply to these if it supports
  bittorrent mainline behaviour?
    - bdmsgs::msgout_find_node() is used!


#### find_node
[1] peer1.msgout_find_node -> [2] peer2.msgin_find_node
[2] peer2.msgout_reply_find_node -> [3] peer1.msgin_reply_find_node

######## [1] peer1.msgout_find_node
< ADD FRIEND >
// I added the friend via UI, or on startup:
// it adds the peer we're interested in to bdNodeManager.mActivePeers
// which is then added to the query queue by bdNodeManager::startQueries
// and hanndled (sent) in bdQueryManager
bdNodeManager::addFindNode() at bdmanager.cc:193 0xd3f79b;
UdpBitDht::addFindNode() at udpbitdht.cc:114 0xd2bdd1;
p3BitDht::findPeer() at p3bitdht_peers.cc:131 0xacb07f;
p3NetMgrIMPL::netAssistFriend() at p3netmgr.cc:1,372 0xc4ba1b;
p3LinkMgrIMPL::addFriend() at p3linkmgr.cc:2,147 0xc449f9;
// query is sent bdQueryManager in bdNodeManager::startQueries()
// and handled by bdQueryManagers thread
bdmsgs::bitdht_find_node_msg(transId, &(mOwnId), query, msg, avail-1);
bdNode::msgout_find_node(id, &transId, targetNodeId);
bdNode::send_query() at bdnode.cc:451 0xd3b70a;
bdQueryManager::iterateQueries() at bdquerymgr.cc:122 0xd35df2;
bdNode::iteration() at bdnode.cc:394 0xd3d2c9;
bdNodeManager::iteration() at bdmanager.cc:495 0xd3f4ff;
UdpBitDht::run() at udpbitdht.cc:327 0xd2cdc7;

######## [2] peer2.msgin_find_node + [2] peer2.msgout_reply_find_node
// libbitdht/src/bitdht/bdnode.cc
recvPkt()
    /****************** Bits Parsed Ok. Process Msg ***********************/
    case BITDHT_MSG_TYPE_FIND_NODE: /* a: id, transId, target */;
        msgin_find_node(&srcId, &transId, &target_info_hash);
        bdNode::msgin_find_node();
            queueQuery(id, query, transId, BD_QUERY_NEIGHBOURS);
                // adds query to mRemoteQueries, which is then handled by different thread
                bdNode::processRemoteQuery();
                    switch(query.mQueryType);
                    case BD_QUERY_NEIGHBOURS:;
                        int bdSpace::find_nearest_nodes();
                        msgout_reply_find_node(&(query.mId), &(query.mTransId), nearList);
                            bdNode::msgout_reply_find_node();
                                int blen = bitdht_resp_node_msg(transId, &(mOwnId), peers, msg, avail-1);
                                sendPkt(msg, blen, id->addr);

            addPeer(id, peerflags);
######## [3]
bdNode::recvPkt();
    case BITDHT_MSG_TYPE_REPLY_NODE
        bdNode::msgin_reply_find_node();
            bdNode::addPeer(id, peerflags);



#### get_hash
[1] peer1.msgout_get_hash -> [2] peer2.msgin_get_hash
[2] peer2.msgout_reply_hash -> [3] peer1.msgin_reply_hash

[1] not implemented or hooked up
[2] peer2.msgin_get_hash
bdNode::recvPkt();
    case BITDHT_MSG_TYPE_GET_HASH:    /* a: id, transId, info_hash */
        msgin_get_hash(&srcId, &transId, &target_info_hash);
            queueQuery(id, info_hash, transId, BD_QUERY_HASH);
                // adds query to mRemoteQueries, which is then handled by different thread
                bdNode::processRemoteQuery();
                    switch(query.mQueryType);
                        TODO: should send a msgout_reply_hash();
[3]
bdNode::recvPkt();
    case BITDHT_MSG_TYPE_REPLY_HASH:
        msgin_reply_hash(&srcId, &transId, &token, values);
            TODO: this does nothing, it should save and use the hash


