/*
 * bitdht/udpbitdht_nettest.cc
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

/*
This test program needs at least two instances to run.
It takes various command line args so that it can look up the other instance.
// first dht
./udpbitdht_msgtest -b localboot_find3074.txt -p3074 -ub8033e8acab57e170b612372727b38a60f28b76e -tb8033e8acab57e170b612372727b38a60f28b76e

// second dht which finds first
./udpbitdht_msgtest -b localboot_find3074.txt -p3099 -b localboot_find3074.txt -t6238303333653861636162353765313730623631

It tests finding a node, posting a hash to that node, and getting that same
hash back from the node.
*/

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "udp/udpbitdht.h"
#include "udp/udpstack.h"
#include "bitdht/bdstddht.h"
#include "bitdht/bdmanager.h"
#include "bitdht/bdiface.h"
#include "bdHandler.h"
#include "auth/Storage.h"
#include "auth/PasswordAuth.h"


#define MAX_MESSAGE_LEN 10240
#define DEF_PORT    7500
#define MIN_DEF_PORT    1001
#define MAX_DEF_PORT    16000
#define DEF_BOOTFILE    "localboot.txt"


bool findNode(BitDhtHandler &bitdhtHandler,
              UdpBitDht *udpBitDht,
              const std::string findPeerName,
              bdId &resultId)
{
    /* install search node */
    bdNodeId searchId;
    if (!bdStdLoadNodeId(&searchId, findPeerName))
    {
        std::cerr << "findNode(): Invalid findPeerName: " << findPeerName;
        return false;
    }

    std::cerr << "searching for Id: ";
    bdStdPrintNodeId(std::cerr, &searchId);
    std::cerr << std::endl;

    // bool BitDhtHandler::FindNode(UdpBitDht &udpBitDht, bdNodeId *peerId)
    bitdhtHandler.FindNode(udpBitDht, &searchId);

    // bdId resultId;
    uint32_t status;
    resultId.id = searchId;

    while(false == bitdhtHandler.SearchResult(&resultId, status))
    {
        sleep(10);
    }

    std::cerr << "findNode(): Found Result:" << std::endl;
    std::cerr << "\tId: ";
    bdStdPrintId(std::cerr, &resultId);
    std::cerr << std::endl;

    return true;
}

std::string getHash(BitDhtHandler &bitdhtHandler,
                    UdpBitDht *bitdht,
                    bdId &targetNode,
                    bdNodeId key)
{
    bitdht->getHash(targetNode, key);

    // check for results
    while(false == bitdhtHandler.m_gotHashResult)
    {
        sleep(10);
    }

    return bitdhtHandler.m_getHashValue;
}


bool postHash(BitDhtHandler &bitdhtHandler,
              UdpBitDht *bitdht,
              bdId &targetNode,
              bdNodeId key,
              std::string hash,
              std::string secret)
{
    bitdht->postHash(targetNode, key, hash, secret);

    // check for results
    while(false == bitdhtHandler.m_postHashGotResult)
    {
        sleep(10);
    }

    return bitdhtHandler.m_postHashSuccess;
}


void test_hashSpace()
{
    bdNodeId id;
    /* start off with a random id! */
    bdStdRandomNodeId(&id);

    // write an entry to the hash space
    uint32_t modFlags = BITDHT_HASH_ENTRY_ADD;
    std::string strKey("key");
    std::string strValue("value");
    std::string strSecret("secret");
    time_t lifetime = 0;
    time_t store = 0;

    bdHashSpace testHashSpace;
    bdHashEntry entry(strValue, strSecret, lifetime, store);

    testHashSpace.modify(&id, strKey, &entry, modFlags);

    testHashSpace.printHashSpace(std::cerr);

    // now find the entry using key lookup
    std::list<bdHashEntry> foundEntries;
    testHashSpace.search(&id, strKey, 0x7FFFFFFF, foundEntries);

    std::list<bdHashEntry>::iterator it;
    for(it = foundEntries.begin(); it != foundEntries.end(); it++)
    {
        assert(it->mValue == strValue);
        // std::cerr << "Found hash:";
        // std::cerr << it->mValue << std::endl;
    }
}

int args(char *name)
{
    std::cerr << "Usage: " << name;
    std::cerr << " -p <port> ";
    std::cerr << " -b </path/to/bootfile> ";
    std::cerr << " -u <uid> ";
    std::cerr << " -q <num_queries>";
    std::cerr << " -r  (do dht restarts) ";
    std::cerr << " -j  (do join test) ";
    std::cerr << " -f do find_host before get_hash? <true|false>";
    std::cerr << " -t <target bdNodeId to perform find_host for, e.g., b8033e8acab57e170b612372727b38a60f28b76e>";
    std::cerr << std::endl;
    return 1;
}

int main(int argc, char **argv)
{
    int c;
    int port = DEF_PORT;
    std::string bootfile = DEF_BOOTFILE;
    std::string uid;
    bool setUid = false;
    bool doRandomQueries = false;
    bool doRestart = false;
    bool doThreadJoin = false;
    int noQueries = 0;
    bool doFindNode = false;
    std::string findPeerName;

    while((c = getopt(argc, argv,"rjfp:b:u:q:t:")) != -1)
    {
        switch (c)
        {
            case 'r':
                doRestart = true;
                break;
            case 'j':
                doThreadJoin = true;
                break;
            case 'p':
            {
                int tmp_port = atoi(optarg);
                if ((tmp_port > MIN_DEF_PORT) && (tmp_port < MAX_DEF_PORT))
                {
                    port = tmp_port;
                    std::cerr << "Port: " << port;
                    std::cerr << std::endl;
                }
                else
                {
                    std::cerr << "Invalid Port";
                    std::cerr << std::endl;
                    args(argv[0]);
                    return 1;
                }

            }
            break;
            case 'b':
            {
                bootfile = optarg;
                std::cerr << "Bootfile: " << bootfile;
                std::cerr << std::endl;
            }
            break;
            case 'u':
            {
                setUid = true;
                uid = optarg;
                std::cerr << "UID: " << uid;
                std::cerr << std::endl;
            }
            break;
            case 'q':
            {
                doRandomQueries = true;
                noQueries = atoi(optarg);
                std::cerr << "Doing Random Queries";
                std::cerr << std::endl;
            }
            break;
            case 'f':
            {
                doFindNode = true;
                std::cerr << "doFindNode: " << doFindNode << std::endl;
            }
            break;
            case 't':
            {
                findPeerName = optarg;
                std::cerr << "findPeerName: " << findPeerName;
                std::cerr << std::endl;
            }
            break;
            default:
            {
                std::cerr << "default = fail?" << std::endl;

                args(argv[0]);
                return 1;
            }
            break;
        }
    }
    if(findPeerName.length() == 0)
    {
        args(argv[0]);
        return 1;
    }

    test_hashSpace();

    bdDhtFunctions *fns = new bdStdDht();

    bdNodeId id;
    /* start off with a random id! */
    bdStdRandomNodeId(&id);
    if (setUid)
    {
        int len = uid.size();
        if (len > 20)
        {
            len = 20;
        }

        for(int i = 0; i < len; i++)
        {
            id.data[i] = uid[i];
        }
    }
    std::cerr << "Using NodeId: ";
    fns->bdPrintNodeId(std::cerr, &id);
    std::cerr << std::endl;

    /* setup the udp port */
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = 0;
    local.sin_port = htons(port);

    UdpStack *udpstack = new UdpStack(local);

    /* create bitdht component */
    // 99 is the application version number I'm using so that other retroshare
    // clients dont't connect.
    std::string dhtVersion = "db99ST";
    UdpBitDht *bitdht = new UdpBitDht(udpstack, &id, dhtVersion,
                                      bootfile, fns);

    /* add in the stack */
    udpstack->addReceiver(bitdht);

    // Make our callback handler class (which also makes the query)
    BitDhtHandler bitdhtHandler;
    bitdht->addCallback(&bitdhtHandler);

    /* startup threads */
    bitdht->start();

    /* setup best mode for quick search */
    uint32_t dhtFlags = BITDHT_MODE_TRAFFIC_HIGH | BITDHT_MODE_RELAYSERVERS_IGNORED;
    bitdht->setDhtMode(dhtFlags);
    bitdht->setAttachMode(false);


    int count = 0;
    int running = 1;

    std::cerr << "Starting Dht: ";
    std::cerr << std::endl;
    bitdht->startDht();

    if (doRandomQueries)
    {
        for(int i = 0; i < noQueries; i++)
        {
            bdNodeId rndId;
            bdStdRandomNodeId(&rndId);

            std::cerr << "BitDht Launching Random Search: ";
            bdStdPrintNodeId(std::cerr, &rndId);
            std::cerr << std::endl;

            bitdht->addFindNode(&rndId, BITDHT_QFLAGS_DO_IDLE);
        }
    }

    // Target node for query. If doing a findNode query, this will be set
    // with the result.
    bdId targetNode;
    if(!doFindNode)
    {
        bdNodeId targetID;
        memcpy(targetID.data, findPeerName.data(), BITDHT_KEY_LEN);
        struct sockaddr_in target_addr;
        memset(&target_addr, 0, sizeof(target_addr));
        target_addr.sin_family = AF_INET;
        char *ip = {"127.0.0.1"};
        target_addr.sin_addr.s_addr = inet_addr(ip);
        target_addr.sin_port = htons(3074);
        bdId hardTargetNode(targetID, target_addr);
        targetNode = hardTargetNode;
    }

    bool useDHT = true;
    Storage storage(useDHT, &bitdhtHandler, bitdht, &targetNode);

    std::string value = "-----BEGIN PGP PUBLIC KEY BLOCK-----\
Version: OpenPGP:SDK v0.9\
\
xsBNBFMYXjUBCACdmfb/fC5u3/oIsbnpKXCqZk3OCx0YSiWAg2SeuyLPj1DES06W\
Yx2eHs0ci7noO6aXbLf0f9+JIUSJUiTdkZZjHBA5FcTy9FbZmlu0zi/Qqs7EXNJT\
tT1BM3JRvIIEBOSlgEKYzJxb0onX4vQ1J1/sQSi1lZUmy0O6svCNmqFg/Kt9Aa3S\
gNaOeaPDr+hAoYpfyp7m5zYsA5r6Rex6O8qRzTqkTYEAtTq5jAms01YrtluD5GNB\
RZuhiXNobfosBueYSuK5KlpOJczn8qViUSEPnybbodcZDZpmcdliyQoIqtJiVRny\
5gMWn08vLkpX9gzz04gNLJvzHN7GWiiP7/G1ABEBAAHNJVJlZENyYWlnIChHZW5l\
cmF0ZWQgYnkgUmV0cm9TaGFyZSkgPD7CwF8EEwECABMFAlMYXjUJEP3jPOYQGt7o\
AhkBAACn6Af9GP/qezpV6+8uO7dcMCen4GwWcKR1OA3haL3KUc8II68aFOoct7qr\
FsFOw6Cn378w3IC3gAGObUKpWYGU/7b6Gh1i6W6whYl7tWFLevhcSkU4fZF9X3PR\
mgs8AiofnubevDGGH6M0YBBAnTdsrUtsm4HRDBMLpitt2SQCYc5gnAUuaCRY63Fg\
Ax+P/Kldeso15+dlrpjGr5xZMDWEubWH2GpELJJSOb1CCC3rANcnxUT18kLFBB2K\
jKSTD9ndswUv4mCH9DIaccfMHO0r2XjevAox7gJRGQpbr0wj79Wkb5JDb8z0PFcK\
FwSK6LclF4xv61JR42mYGMEYbPSu4el1Sw==\
=2kN4\
-----END PGP PUBLIC KEY BLOCK-----\
--SSLID--660c5d8193c238f2b661aa6715da2338;--LOCATION--laptop;\
--LOCAL--192.168.1.104:2191;--EXT--12.34.56.789:2191;\
\0";

    bool doFindPostGet = false;
    bool foundNode = false;
    bool sentGetHash = false;
    bool sentPostHash = false;
    while(1)
    {
        sleep(10);

        std::cerr << "BitDht State: ";
        std::cerr << bitdht->stateDht();
        std::cerr << std::endl;
        if(bitdht->stateDht() == BITDHT_MGR_STATE_ACTIVE)
        {

            std::cerr << "BITDHT_MGR_STATE_ACTIVE" << std::endl;
            // bitdht->printDht();

            bdId resultId;
            if(doFindPostGet && doFindNode && foundNode == false)
            {
                findNode(bitdhtHandler, bitdht, findPeerName, resultId);

                std::cerr << "bdSingleShotFindPeer(): Found Result:" << std::endl;
                std::cerr << "\tId: ";
                bdStdPrintId(std::cerr, &resultId);
                std::cerr << std::endl;
                std::cerr << "Answer: ";
                std::cerr << std::endl;
                std::cerr << "\tPeer IpAddress: " << bdnet_inet_ntoa(resultId.addr.sin_addr);
                std::cerr << std::endl;
                std::cerr << "\tPeer Port: " << ntohs(resultId.addr.sin_port);
                std::cerr << std::endl;

                foundNode = true;
            }

            // post_hash
            if(doFindPostGet && !sentPostHash)
            {
                // If we've done the find_node request, then use it's
                // result as the targetNode.
            	if(foundNode)
            	{
            		targetNode = resultId;
            	}

                bdNodeId key;
                memcpy(key.data, "USERNAME_CRAIG------", 20);

                std::string secret = "i am a secret";
                // When this finishes, bitdhtHandler.m_postHashSuccess
                // indicates success.
                postHash(bitdhtHandler, bitdht, targetNode, key, value, secret);
                sentPostHash = true;
            }
            // get_hash
            if(doFindPostGet && !sentGetHash)
            {
                // If we've done the find_node request, then use it's
                // result as the targetNode.
            	if(foundNode)
				{
					targetNode = resultId;
				}

                bdNodeId key;
                memcpy(key.data, "USERNAME_CRAIG------", 20);
                // When this finishes, the hash will be present in:
                // bitdhtHandler.m_getHashValue.
                std::string hash = getHash(bitdhtHandler, bitdht,
                                           targetNode, key);
                sentGetHash = true;
                assert(memcmp(value.data(), hash.data(), 1018) == 0);
            }

            // register account using password based auth
            unsigned int usernameLen = 20;
            char username[USERNAME_LEN] = "username____________";
            unsigned int passwordLen = 20;
            char password[PASSWORD_LEN] = "my_password_________";

            char pgpkey[PGP_PUB_KEY_LEN] = "-----BEGIN PGP PUBLIC KEY BLOCK-----\
Version: OpenPGP:SDK v0.9\
\
xsBNBFMYXZ4BCADRI6QmxQ83NStCZ2ryPsNxw70IDJLmYOpuAI3eS+mUUWeRwP8M\
RLjjOgYnL3Ij0sp/ZTDTOnPQoYNmiEKu3altvR07jNXMaqz2KVSDQnl+ZKrRqnhd\
H18WRqOAUvMcDynSMr3tipnVMaDSh1on2l2jE75IxXjxXYTjxM/GtP4ZjuBFVnT7\
8tjhDyk9YFYzLZoWDEcMqQ8JGyzJmwLGrxs5OTCXPInUKTyS/62FEWHKuGbK2cKG\
Ky4WR+SN4w0OdnzLpgg5EUulJp+IP+JzJmGGhmVJRugCho3hMt1T0W6c4d4vUX4R\
djOyhnveG7z3Fno6xXimrh3h6liX0AW1Had/ABEBAAHNJVJlZENyYWlnIChHZW5l\
cmF0ZWQgYnkgUmV0cm9TaGFyZSkgPD7CwF8EEwECABMFAlMYXZ4JECSqjas2zLG2\
AhkBAADqFggAhzCGB4DymuuUlMo0CseCif/czmAt/1ScY8AowNG7XswQ0bXRs15o\
jSad3LD5qsEeGv2NKmVxxkymysnBaousVYjeiSGAy/qp9H+tSWMwxWZ5Dx+htcmy\
MvnX1+BH1tqSa3r/w2VzSIQQcixcDTzxmr74juDcyeiMfKpiRjh/hUZuz4c+YTC1\
F6ZXxcFCaBZ+3q9hEmAM7v0TRGivoItrQxm46+wvEzigg8E7lWEfSDPHJe942ZyU\
pC6Z4oa/jtbEHXxYNdTEYIIcJiIJNmdUydRWhtAbOIZ8G7gJCIzSKXPZYZH8Fni7\
xBJCZgifpsBXqa7Dq9MYmVW2epPeDg19lw==\
=2MaV\
-----END PGP PUBLIC KEY BLOCK-----\
\
-----BEGIN PGP PRIVATE KEY BLOCK-----\
Version: OpenPGP:SDK v0.9\
\
xcL9BFMYXZ4BCADRI6QmxQ83NStCZ2ryPsNxw70IDJLmYOpuAI3eS+mUUWeRwP8M\
RLjjOgYnL3Ij0sp/ZTDTOnPQoYNmiEKu3altvR07jNXMaqz2KVSDQnl+ZKrRqnhd\
H18WRqOAUvMcDynSMr3tipnVMaDSh1on2l2jE75IxXjxXYTjxM/GtP4ZjuBFVnT7\
8tjhDyk9YFYzLZoWDEcMqQ8JGyzJmwLGrxs5OTCXPInUKTyS/62FEWHKuGbK2cKG\
Ky4WR+SN4w0OdnzLpgg5EUulJp+IP+JzJmGGhmVJRugCho3hMt1T0W6c4d4vUX4R\
djOyhnveG7z3Fno6xXimrh3h6liX0AW1Had/ABEBAAH+AwEC8oCT3eeDn7oAAAAA\
AAAAAPCwh6qltznpzLDAcKoU/u2naJFSFd2+T7oUiI2bGIMyiDZ/a1Z6tmOCuIhC\
UHtl+vV0dycfkfg2a51O6VwdZmTM/f7jPWXjX0oGoJ54caPZDrrvOSH1ssgXb65l\
2W89BjCffmB7BjKQIzn4mq2kXaXCx/iGyrIS8LKB+6StY8rmEu5VEGwpaeZ3s9Gn\
ERiVQU7NKiCFGa72fisfsIUHMPRLLr1ulgyQdL3ITdY6Nx79UVlrdImO6MPzAUF8\
gnLhArzXEvaXmSLuuq2EIJmdSxUBkrm31hDI0zDQdMGnVaMGthazqy6JPSE5PYAn\
RknOPoyTJOrgxBWN26F64e655O48frAZ4YgJr6Tu7OJpena4vjTM+Nz7jtW/CDK6\
DvrjD2HcZlcDcrYsa54SGaBkfZPW8H4fER/1H8OKH679Mbyez9qIpTWfQTexPhLd\
QTTBCVAp0TYDixwVh+2xfL4fp+SZbc/b872QJ1Pfu9XMqugrOZA/NP4VYYhUE3g4\
oc9pwxv1WDAWbq+w/dFBpAdapb+qw7S3JTEVkw9ngmdcPUxpSE72q1puka72Qn65\
HmCRmQVYzseFHFLDkME4HvGhKXpC05tv7rsqi9BGi74Ph8injXrtNuKbXYbTs0cK\
529905l6hGNX23CFGojTYOJgtkGQ37dDlLD9k7A7KiXsD3kPAajxk7j/g7GqpU2S\
YuLMj4DdZ6WyZtXcLF5emI6CO6HwNNwRGtcFUkwFoLuPzhIZJe9Z1FAFS4xoWMIY\
+7YxCnkKEzdiKXLz8BPqCNBdxLOD04GgZEE5VaWjKRzWgiWlrBLqoN2M7IR7MB30\
rzo7c04XRGTxrW4rxFf1yoRo1r9Ug/GxK21FprFpWdlvoiPoUeuojUVjbU41ie5H\
zSVSZWRDcmFpZyAoR2VuZXJhdGVkIGJ5IFJldHJvU2hhcmUpIDw+wsBfBBMBAgAT\
BQJTGF2eCRAkqo2rNsyxtgIZAQAA6hYIAIcwhgeA8prrlJTKNArHgon/3M5gLf9U\
nGPAKMDRu17MENG10bNeaI0mndyw+arBHhr9jSplccZMpsrJwWqLrFWI3okhgMv6\
qfR/rUljMMVmeQ8fobXJsjL519fgR9bakmt6/8Nlc0iEEHIsXA088Zq++I7g3Mno\
jHyqYkY4f4VGbs+HPmEwtRemV8XBQmgWft6vYRJgDO79E0Ror6CLa0MZuOvsLxM4\
oIPBO5VhH0gzxyXveNmclKQumeKGv47WxB18WDXUxGCCHCYiCTZnVMnUVobQGziG\
fBu4CQiM0ilz2WGR/BZ4u8QSQmYIn6bAV6muw6vTGJlVtnqT3g4NfZc=\
=4LBP\
-----END PGP PRIVATE KEY BLOCK-----\
";
            unsigned int pgpkeyLen = 2828;

            registerAccount(&storage,
                            username, usernameLen,
                            password, passwordLen,
                            pgpkey, pgpkeyLen);


            // Now login with password auth using those account details
            char readPGPkey[PGP_KEY_LEN];
            unsigned int readPGPkeyLen = PGP_KEY_LEN;
            interactiveLogin(&storage, username, password, passwordLen,
                             readPGPkey, readPGPkeyLen);
            assert(memcmp(pgpkey, readPGPkey, readPGPkeyLen) == 0);
        }

        std::cerr << "Dht Network Size: ";
        std::cerr << bitdht->statsNetworkSize();
        std::cerr << std::endl;

        std::cerr << "BitDht Network Size: ";
        std::cerr << bitdht->statsBDVersionSize();
        std::cerr << std::endl;


        if (doThreadJoin)
        {
            /* change address */
            if (count % 2 == 0)
            {
                std::cerr << "Resetting UdpStack: ";
                std::cerr << std::endl;

                udpstack->resetAddress(local);
            }
        }
        if (doRestart)
        {
            if (count % 2 == 1)
            {
                if (running)
                {
                    std::cerr << "Stopping Dht: ";
                    std::cerr << std::endl;

                    bitdht->stopDht();
                    running = 0;
                }
                else
                {
                    std::cerr << "Starting Dht: ";
                    std::cerr << std::endl;

                    bitdht->startDht();
                    running = 1;
                }
            }
        }
    }

    return 1;
}
