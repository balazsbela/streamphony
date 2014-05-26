/*
 * Author: Craig McInnes
 * Date: 10/03/2014
 *
 * Contains generic somewhat crypto related fns for auth.
*/
#include "AuthCryptoFns.h"

#include <assert.h>
#include <openssl/sha.h>
#include <fstream>      // std::ifstream
#include <string.h> // memset, memcpy

#include "rsaes.h"

void generateKey(char *key, int keylen)
{
    // TODO: not cross platform, unix only
    std::ifstream random("/dev/urandom", std::ios_base::in);
    random.read(reinterpret_cast<char*>(key), keylen);
    random.close();
}

void encrypt(char* key,
             char* const dataToEncrypt,
             const unsigned int dataToEncryptLen,
             char* const encryptedData,
             unsigned int &encryptedDataLen)
{
    // static bool   aes_crypt_8_16(const uint8_t *input_data,
    //                              uint32_t input_data_length,
    //                              uint8_t key[16],
    //                              uint8_t salt[8],
    //                              uint8_t *output_data,
    //                              uint32_t& output_data_length);
    // static bool aes_decrypt_8_16(const uint8_t *input_data,
    //                              uint32_t input_data_length,
    //                              uint8_t key[16],
    //                              uint8_t salt[8],
    //                              uint8_t *output_data,
    //                              uint32_t& output_data_length);

    // Encrypt concatenated data [salt||encrypt(KLI, (fKS||KKS||KW))] using
    // using symmetric key KLI (aes encrypt/decrypt).
    RsAES::aes_crypt_8_16(reinterpret_cast<unsigned char*>(dataToEncrypt),
                          dataToEncryptLen,
                          reinterpret_cast<unsigned char*>(key),
                          NULL,
                          reinterpret_cast<unsigned char*>(encryptedData),
                          encryptedDataLen);
}

void decrypt(const char* const key,
             char* const dataToDecrypt,
             const unsigned int dataToDecryptLen,
             char* const decryptedData,
             unsigned int &decryptedDataLen)
{
    // static bool   aes_crypt_8_16(const uint8_t *input_data,
    //                              uint32_t input_data_length,
    //                              uint8_t key[16],
    //                              uint8_t salt[8],
    //                              uint8_t *output_data,
    //                              uint32_t& output_data_length);
    // static bool aes_decrypt_8_16(const uint8_t *input_data,
    //                              uint32_t input_data_length,
    //                              uint8_t key[16],
    //                              uint8_t salt[8],
    //                              uint8_t *output_data,
    //                              uint32_t& output_data_length);

    unsigned char newkey[16];
    memcpy(newkey, key, 16);

    // Decrypt concatenated data [salt||Decrypt(KLI, (fKS||KKS||KW))] using
    // using symmetric key KLI (aes Decrypt/decrypt).
    bool ok = RsAES::aes_decrypt_8_16(reinterpret_cast<unsigned char*>(dataToDecrypt),
                                      dataToDecryptLen,
                                      newkey,
                                      NULL,
                                      reinterpret_cast<unsigned char*>(decryptedData),
                                      decryptedDataLen);
    if(!ok)
    {
        printf("AES decrypt failed.");
        assert(ok == true);
    }
}

unsigned int generateSalt()
{
    // TODO: not cross platform, unix only
    std::ifstream random("/dev/urandom", std::ios_base::in);
    unsigned int salt;
    random.read(reinterpret_cast<char*>(&salt), sizeof(salt));
    random.close();
    return salt;
}

void keyDerivationFunction(const unsigned int salt,
                           const char* const password, const int passwordLen,
                           char* key, int keylen)
{
    // Object to hold the current state of the hash
    SHA_CTX ctx;
    SHA1_Init(&ctx);
    // Hash each piece of data as it comes in:
    SHA1_Update(&ctx, (char*)&salt, sizeof(salt));
    // SHA1_Update(&ctx, "hell", 4);
    SHA1_Update(&ctx, password, passwordLen);
    // unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1_Final(reinterpret_cast<unsigned char*>(key), &ctx);
}
