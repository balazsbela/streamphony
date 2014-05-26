/*
 * Author: Craig McInnes
 * Date: 10/03/2014
 *
 * IO fns for auth.
*/
#include <fstream>      // std::ifstream, std::ofstream
#include <iostream>     // cout, endl
#include <cstring>      // strlen
#include "Storage.h"
#include "bitdht/bdiface.h"


Storage::Storage(bool useDHT,
                 BitDhtHandler *const bitdhtHandler,
                 UdpBitDht *const bitdht,
                 bdId *const targetNode)
{
    mUseDHT = useDHT;
    if(mUseDHT == true)
    {
        mBitdhtHandler = bitdhtHandler;
        mBitdht = bitdht;
        mTargetNode = targetNode;
    }
}

bool Storage::writeFKSFile(char* data, int dataLen, char* filename,
                           unsigned int &filenameLen)
{
    // storage engine determines filename
    filenameLen = 20;
    memcpy(filename, "dht_FKS_filename1234", filenameLen);
    filename[filenameLen] = '\0';
    return writeFile(data, dataLen, filename);
}

bool Storage::writeFile(char* data, int dataLen, char* filename)
{
    if(mUseDHT == true)
    {
        bdNodeId key;
        memcpy(key.data, filename, BITDHT_KEY_LEN);

        std::string value(data, dataLen);
        // string::string(const char *szCString, size_type unLength)

        // memcpy attempt
        // std::string value;
        // memcpy(value.data, data, dataLen);

        // no secret for now, blank string
        std::string secret;

        return postDHTValue(*mTargetNode, key, value, secret);
    }
    return writeFileToDisk(data, dataLen, filename);
}

bool Storage::writeFileToDisk(char* data, int dataLen, char* filename)
{
    std::ofstream outfile;
    outfile.open(filename, std::ios_base::trunc);
    outfile.write(data, dataLen);
    outfile.close();
    return true;
}

bool Storage::writeMetadataFile(char* metadataBuf, const unsigned int metadataLen,
                                char* const filename, unsigned int &filenameLen)
{
    // storage engine determines filename
    filenameLen = 20;
    memcpy(filename, "auth_metadata1234567", filenameLen);
    filename[filenameLen] = '\0';
    return writeFile(metadataBuf, metadataLen, filename);
}


bool Storage::readFile(const char* const filename,
                       char* buf,
                       unsigned int &bufLen)
{
    if(mUseDHT == true)
    {
        bdNodeId key;
        memcpy(key.data, filename, BITDHT_KEY_LEN);

        std::string value;

        bool ok = getDHTValue(*mTargetNode, key, value);
        if(ok)
        {
            memcpy(buf, value.data(), value.size());
            bufLen = value.size();
        }
        return ok;
    }
    return readFileFromDisk(filename, buf, bufLen);
}

bool Storage::readFileFromDisk(const char* const filename,
                               char* buf,
                               unsigned int &bufLen)
{
    unsigned int position = 0;
    std::ifstream infile(filename);
    if(infile.is_open())
    {
        while(!infile.eof() && position < bufLen)
        {
            infile.get(buf[position]);
            position++;
        }
        buf[position-1] = '\0';
        bufLen = position-1;
    }
    else
    {
        std::cout << "File" << filename << "could not be opened." << std::endl;
    }
    return true;
}

bool Storage::postDHTValue(bdId &targetNode,
                           bdNodeId key,
                           std::string hash,
                           std::string secret)
{
    mBitdht->postHash(targetNode, key, hash, secret);

    // check for results
    while(false == mBitdhtHandler->m_postHashGotResult)
    {
        sleep(2);
    }

    bool status = mBitdhtHandler->m_postHashSuccess;
    mBitdhtHandler->clearResult();
    return status;
}


bool Storage::getDHTValue(bdId &targetNode,
                          bdNodeId key,
                          std::string &value)
{
    mBitdht->getHash(targetNode, key);

    // check for results
    while(false == mBitdhtHandler->m_gotHashResult)
    {
        sleep(2);
    }
    value = mBitdhtHandler->m_getHashValue;
    mBitdhtHandler->clearResult();
    return true;
}
