#ifndef ENCODE_DECODE_H
#define ENCODE_DECODE_H

#include "server_messages.h"
#include <boost/log/trivial.hpp>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <string>
#include <fstream>
#include <vector>
// Generate RSA key pair
RSA *generateRSAKeyPair();

// Encrypt plaintext using AES with the provided plaintext
int encryptAES(const std::string &plaintext, const unsigned char *aesKey, std::string &result);

// Decrypt ciphertext using AES with the provided ciphertext
int decryptAES(const std::string &ciphertext, const unsigned char *aesKey, std::string &result);

// Encrypt plaintext using RSA public key
int encryptRSA(const unsigned char *plaintext, RSA *publicKey, std::string &result);

// Decrypt AES key using RSA private key
int decryptRSA(const std::string &ciphertext, RSA *privateKey, std::string &result);

// Convert RSA public key to PEM format string
std::string rsaPublicKeyToString(RSA *publicKey);

// Convert PEM format string to RSA public key
RSA *stringToRSAPublicKey(const std::string &publicKeyString);

//this is for base64 encode/decoding.....
typedef unsigned char BYTE;

extern const std::string base64_chars;

bool is_base64(BYTE c);

std::string base64_encode(BYTE const* buf, unsigned int bufLen);
std::vector<BYTE> base64_decode(std::string const& encoded_string);

void encodeFileToFile(const std::string& inputFilename, const std::string& outputFilename);
void decodeFileToFile(const std::string& inputFilename, const std::string& outputFilename);


// Usage
//---------------------------------------------------------------------------------------------------
// int main()
// {
//     // Generate RSA key pair
//     RSA *publicKey, *privateKey;
//     publicKey = generateRSAKeyPair();
//     privateKey = RSAPrivateKey_dup(publicKey);

//     // Example data
//     std::string plaintext = "Hello World";
//     std::string ciphertext;

//     // Generate a random AES key
//     unsigned char aesKey[AES_BLOCK_SIZE];
//     if (RAND_bytes(aesKey, AES_BLOCK_SIZE) < 0)
//     {
//         std::cerr << "Error generating random AES key." << std::endl;
//         return 1;
//     }

//     // Simulating the tranfer through TCP Boost/Asio Write and Read
//     std::string publicKeyRSAString = rsaPublicKeyToString(publicKey);
//     publicKey = stringToRSAPublicKey(publicKeyRSAString);

//     // Encrypt data with AES using the random key
//     encryptAES(plaintext, aesKey, ciphertext);
//     std::cout << "Encrypted with AES: " << ciphertext << std::endl;

//     // Decrypt data with AES using the decrypted key
//     decryptAES(ciphertext, reinterpret_cast<const unsigned char *>(aesKey), plaintext);
//     std::cout << "Original Text: " << plaintext << std::endl;

//     // Encrypt AES key with RSA public key
//     // std::string ciphertext;
//     encryptRSA(reinterpret_cast<const unsigned char *>(plaintext.c_str()), publicKey, ciphertext);
//     std::cout << "Encrypted with RSA Pupblic Key: " << ciphertext << std::endl;

//     // Decrypt AES key with RSA private key
//     // std::string plaintext;
//     decryptRSA(ciphertext, privateKey, plaintext);
//     std::cout << "Original Text: " << plaintext << std::endl;

//     // Cleanup
//     RSA_free(publicKey);
//     RSA_free(privateKey);

//     return 0;
// }
//---------------------------------------------------------------------------------------------------



#endif