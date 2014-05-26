/*
 * Author: Craig McInnes
 * Date: 10/03/2014
 *
 * Passwords paper implementation:
 * http://dx.doi.org/10.1109/P2P.2012.6335797
*/

#ifndef PASSWORD_AUTH_H
#define PASSWORD_AUTH_H

#include "Storage.h"

#define USERNAME_LEN 64
#define PASSWORD_LEN 64
#define KEY_LEN 16
#define PGP_PUB_KEY_LEN 1024*5
#define PGP_KEY_LEN PGP_PUB_KEY_LEN+2048
#define FKS_ENCRYPTED_DATA_LEN 1024*5
#define FILE_NAME_LEN 21
#define METADATA_SIZE 1024*5



// Entry point for passwords auth. Main logic is here.
bool auth();

// Creates all the necessary parts for an account, writes them using Storage.h.
void registerAccount(Storage *storage,
                     char* username, unsigned int usernameLen,
                     char* password, unsigned int passwordLen,
                     char* pgpkey, unsigned int pgpkeyLen);

// Login
void interactiveLogin(Storage *storage,
                      char* username,
                      char* password, unsigned int passwordLen,
                      char* const PGP, unsigned int &PGPLen);

// loads metadata from disk
// gets the salt, and decrypts the rest using KLI
void getMetadata(Storage *storage,
                 char* const metadataFileName,
                 const char* const password,
                 const unsigned int passwordLen,
                 unsigned int &salt,
                 char* const FKS,
                 unsigned int &FKSLen,
                 char* const KKS,
                 unsigned int &KKSLen,
                 char* const KW,
                 unsigned int &KWLen);

// Reads key store file from disk, decrypts it using 'key' and returns contents
// (which is the PGP key pair) in outbuf
void getPGPKey(Storage *storage,
               const char* const key,
               const char* const filenameKS, const unsigned int filenameKSLen,
               char* const outbuf, unsigned int &outbufLen);

// Assemble the metadata file, concatenates filenameFKS, KKS and KW into
// a single buffer then encrypts that buffer using KLI as the hash.
// Prefixes the buffer with the salt.
// Puts the final data into outbuf.
void packMetaDataFile(unsigned int salt,
                      char* KLI, unsigned int KLILen,
                      char* filenameFKS, unsigned int filenameLen,
                      char* KKS, unsigned int KKSLen,
                      char* KW, unsigned int KWLen,
                      char* outbuf, unsigned int &outbufLen);

// Given a password and buffer data, unpack the buffer and decrypt the content
// into their respective buffers.
void unpackMetaDataFile(const char* const password, const unsigned int passwordLen,
                        const char* const data, const unsigned int dataLen,
                        unsigned int &salt,
                        char* FKS, unsigned int &FKSLen,
                        char* KKS, unsigned int &KKSLen,
                        char* KW, unsigned int &KWLen);

// Writes data array into outbuf. Data format is:
// [dataLen, [byte data]]
// N.B.
// usedOutBufLen[out] returns the length of the array data AND the four bytes
//                    that were used to write the length of the array data
//                    which were written at the start of the array data.
char* writeArray(const char* const data, const unsigned int dataLen,
                 char* const outbuf, unsigned int& usedOutBufLen);

// reads the array in data into outbuf. data format is expected to be:
// [dataLen, [byte data]]
// N.B.
// usedOutBufLen[out] returns the length of the array data that was read
//                    This does _not_ include the 4 bytes which it read from
//                    buffer to deserialize the length of the array data.
const char* readArray(const char* const data,
                      char* const outbuf,
                      unsigned int &usedOutBufLen);

void test_readWriteArray();

/*
Algorithm 1 Account Registration
1: uname ← User.input(“Choose username:”)
2: passwd ← User.input(“Choose strong password:”)
3: KK S ← generateKey()
4: FKS ← encryptKKS (Kx1||Kx2|| . . .)
5: fK S ← Storage.create(FK S )
6: salt ← generateSalt()
7: devmap ← createMap()
8: KLI ← KDF(salt,passwd)
9: KW ← generateKey() // suitable for the storage system
10: FLI ← salt||encryptKLI (fKS||KKS||KW ||devmap)
11: fLI ← Storage.create(FLI ) // using KW
12: while DHT.put(uname,fLI ) fails
13: uname ← User.input(“Choose new username:”)
14: end while
*/

/*
Algorithm 2 Login
1: fDL, KDL ← Device.readLocalStore()
2: if fDL "= NULL then // non-interactive login
3: FDL ← Storage.read(fDL)
4: fKS, KKS ← decryptKDL(FDL)
5: saveLoginLocally ← False
6: else // interactive login
7: uname ← User.input(“Enter username:”)
8: passwd ← User.input(“Enter password:”)
9: saveLoginLocally ← User.input(“Remember?”)
10: fLI ← DHT.get(uname)
11: FLI ← Storage.read(fLI)
12: salt ← FLI.salt // stored in plaintext
13: KLI ← KDF(salt,passwd)
14: fKS, KKS, KW, devmap ← decryptKLI (FLI)
15: end if
16: FKS ← Storage.read(fKS)
17: Kx1, Kx2,... ← decryptKKS (FKS)
18: if saveLoginLocally then
19: KDL ← generateKey()
20: FDL ← encryptKDL(fKS||KKS)
21: fDL ← Storage.create(FDL)
22: Device.writeLocalStore(fDL||KDL)
23: devmap.append(Device.ID, fDL||KDL)
24: FLI ← salt||encryptKLI (fKS||KKS||KW||devmap)
25: Storage.write(fLI,FLI) // using KW
26: end if
*/

#endif // PASSWORD_AUTH_H
