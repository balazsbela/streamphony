/*
 * Author: Craig McInnes
 * Date: 10/03/2014
 *
 * IO fns for auth.
 * TODO: Currently writes to disk, update to use DHT.
*/

#ifndef STORAGE_H
#define STORAGE_H

#include <list>
#include <string>  // string, strlen

#include "bitdht/bdnode.h"
#include "udp/udpbitdht.h"
#include "bdHandler.h"
#include <unistd.h>


class Storage
{
public:
    Storage(bool useDHT,
            BitDhtHandler *const bitdhtHandler=NULL,
            UdpBitDht *const bitdht=NULL,
            bdId *const targetNode=NULL);

    // Specifically writes the key store file (FKS)to disk. FKS is the encrypted
    // PGP file.
    // This function gives the FKS file a filename, and sets the
    // char* filename to that value.
    bool writeFKSFile(char* data,
                      int dataLen,
                      char* filename,
                      unsigned int &filenameLen);

    // Specifically writes the Metadata file (FLI) to disk.
    // This function gives the FKS file a filename, and sets the
    // char* filename to that value.
    bool writeMetadataFile(char* metadataBuf,
                           const unsigned int metadataLen,
                           char* const filename,
                           unsigned int &filenameLen);

    // Calls into writeFileToDisk or postDHTValue depending if mUseDHT is set.
    bool writeFile(char* data, int dataLen, char* filename);

    // Calls readFileFromDisk or getDHTValue depending if mUseDHT is set.
    bool readFile(const char* const filename,
                  char* buf,
                  unsigned int &bufLen);

private:
    bool writeFileToDisk(char* data, int dataLen, char* filename);

    bool postDHTValue(bdId &targetNode,
                      bdNodeId key,
                      std::string hash,
                      std::string secret);

    // bufLen[in] the length of buf that can be used
    // bufLen[out] the length of the data that was written to buf
    bool readFileFromDisk(const char* const filename,
                          char* buf, unsigned int &bufLen);

    bool getDHTValue(bdId &targetNode,
                     bdNodeId key,
                     std::string &value);

    bool mUseDHT;
    BitDhtHandler *mBitdhtHandler;
    UdpBitDht *mBitdht;
    bdId *mTargetNode;
};

#endif //STORAGE_H
