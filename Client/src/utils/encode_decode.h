#ifndef ENCODE_DECODE_H
#define ENCODE_DECODE_H

#include "client_messages.h"
#include <boost/log/trivial.hpp>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

// Generate RSA key pair
RSA *generateRSAKeyPair()
{
    EVP_PKEY *evpKey = EVP_PKEY_new();
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);

    if (EVP_PKEY_keygen_init(ctx) <= 0 ||
        EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0 ||
        EVP_PKEY_keygen(ctx, &evpKey) <= 0)
    {
        BOOST_LOG_TRIVIAL(error) << "Error generating RSA key pair." << std::endl;
        return nullptr;
    }

    EVP_PKEY_CTX_free(ctx);

    RSA *rsaKey = EVP_PKEY_get1_RSA(evpKey);
    EVP_PKEY_free(evpKey);

    return rsaKey;
}

// Encrypt plaintext using AES with the provided plaintext
int encryptAES(const std::string &plaintext, const unsigned char *aesKey, std::string &result)
{
    AES_KEY aesKeyStruct;
    AES_set_encrypt_key(aesKey, 128, &aesKeyStruct);

    std::string ciphertext;

    // Add random IV (Initialization Vector)
    unsigned char iv[AES_BLOCK_SIZE];
    if (RAND_bytes(iv, AES_BLOCK_SIZE) != 1)
    {
        BOOST_LOG_TRIVIAL(error) << "Error generating random IV.";
        return ENCRYPT_FAIL;
    }

    ciphertext += std::string(reinterpret_cast<char *>(iv), AES_BLOCK_SIZE);

    // Encrypt the plaintext
    size_t length = plaintext.length();
    size_t paddedLength = ((length / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;
    unsigned char *encryptedData = new unsigned char[paddedLength];

    AES_cbc_encrypt(reinterpret_cast<const unsigned char *>(plaintext.c_str()),
                    encryptedData, length, &aesKeyStruct, iv, AES_ENCRYPT);

    ciphertext += std::string(reinterpret_cast<char *>(encryptedData), paddedLength);

    delete[] encryptedData;

    // Copy to the result parameter
    result = ciphertext;
    return ENCRYPT_SUCCESS;
}

// Decrypt ciphertext using AES with the provided ciphertext
int decryptAES(const std::string &ciphertext, const unsigned char *aesKey, std::string &result)
{
    AES_KEY aesKeyStruct;

    // Attempt to initialize the AES key for decryption
    if (AES_set_decrypt_key(aesKey, 128, &aesKeyStruct) != 0)
    {
        // Error handling: Key initialization failed
        BOOST_LOG_TRIVIAL(error) << "AES key initialization failed";
        return DECRYPT_FAIL;
    }

    std::string decryptedText;

    // Extract IV from ciphertext
    unsigned char iv[AES_BLOCK_SIZE];
    ciphertext.copy(reinterpret_cast<char *>(iv), AES_BLOCK_SIZE, 0);

    // Perform decryption (No direct error checking available)
    size_t length = ciphertext.length() - AES_BLOCK_SIZE;
    unsigned char *decryptedData = new unsigned char[length];

    // Performing decryption, no direct return value for success or failure
    AES_cbc_encrypt(reinterpret_cast<const unsigned char *>(ciphertext.c_str() + AES_BLOCK_SIZE),
                    decryptedData, length, &aesKeyStruct, iv, AES_DECRYPT);

    // Here, direct error checking isn't possible within the function

    // Assuming successful decryption, convert decrypted data and copy to result
    decryptedText = std::string(reinterpret_cast<char *>(decryptedData), length);
    delete[] decryptedData;

    // Copy decrypted data to the result parameter
    result = decryptedText;
    return DECRYPT_SUCCESS; // Return on successful decryption
}

// Encrypt plaintext using RSA public key
int encryptRSA(const unsigned char *plaintext, RSA *publicKey, std::string &result)
{
    int rsaLen = RSA_size(publicKey);
    unsigned char *encryptedText = new unsigned char[rsaLen];

    // Encrypt the plaintext using the RSA public key
    int status = RSA_public_encrypt(AES_BLOCK_SIZE, plaintext, encryptedText, publicKey, RSA_PKCS1_OAEP_PADDING);

    if (status == -1)
    {
        // Error handling in case of encryption failure
        BOOST_LOG_TRIVIAL(error) << "Error encrypting plaintext with RSA." << std::endl;
        delete[] encryptedText;
        return ENCRYPT_FAIL; // Return failure code
    }

    std::string encryptedTextStr;
    try
    {
        // Convert the encrypted text to a string
        encryptedTextStr.assign(reinterpret_cast<char *>(encryptedText), status);
    }
    catch (const std::exception &e)
    {
        // Error handling if the conversion to string fails
        BOOST_LOG_TRIVIAL(error) << "Error converting encrypted text to string: " << e.what() << std::endl;
        delete[] encryptedText;
        return ENCRYPT_FAIL; // Return failure code
    }

    delete[] encryptedText; // Clean up memory

    // Copy encrypted text to the result parameter
    result = encryptedTextStr;
    return ENCRYPT_SUCCESS; // Return success code
}

// Decrypt AES key using RSA private key
int decryptRSA(const std::string &ciphertext, RSA *privateKey, std::string &result)
{
    int rsaLen = RSA_size(privateKey);
    unsigned char *decryptedText = new unsigned char[rsaLen];

    // Decrypt the ciphertext using the RSA private key
    int status = RSA_private_decrypt(ciphertext.size(),
                                     reinterpret_cast<const unsigned char *>(ciphertext.c_str()),
                                     decryptedText, privateKey, RSA_PKCS1_OAEP_PADDING);

    if (status == -1)
    {
        // Error handling in case of decryption failure
        BOOST_LOG_TRIVIAL(error) << "Error decrypting ciphertext with RSA." << std::endl;
        delete[] decryptedText;
        return DECRYPT_FAIL; // Return failure code
    }

    std::string decryptedTextStr;
    try
    {
        // Convert the decrypted text to a string
        decryptedTextStr.assign(reinterpret_cast<char *>(decryptedText), status);
    }
    catch (const std::exception &e)
    {
        // Error handling if the conversion to string fails
        BOOST_LOG_TRIVIAL(error) << "Error converting decrypted text to string: " << e.what() << std::endl;
        delete[] decryptedText;
        return DECRYPT_FAIL; // Return failure code
    }

    delete[] decryptedText; // Clean up memory

    // Copy decrypted text to the result parameter
    result = decryptedTextStr;
    return DECRYPT_SUCCESS; // Return success code
}

// Convert RSA public key to PEM format string
std::string rsaPublicKeyToString(RSA *publicKey)
{
    BIO *bio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(bio, publicKey);

    char *buffer;
    size_t size = BIO_get_mem_data(bio, &buffer);
    std::string result(buffer, size);

    BIO_free(bio);

    return result;
}

// Convert PEM format string to RSA public key
RSA *stringToRSAPublicKey(const std::string &publicKeyString)
{
    BIO *bio = BIO_new_mem_buf(publicKeyString.c_str(), -1);
    RSA *publicKey = PEM_read_bio_RSAPublicKey(bio, nullptr, nullptr, nullptr);

    BIO_free(bio);

    return publicKey;
}

// Convert RSA private key to PEM format string
std::string rsaPrivateKeyToString(RSA *privateKey)
{
    BIO *bio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPrivateKey(bio, privateKey, nullptr, nullptr, 0, nullptr, nullptr);

    char *buffer;
    size_t size = BIO_get_mem_data(bio, &buffer);
    std::string result(buffer, size);

    BIO_free(bio);

    return result;
}

// Convert PEM format string to RSA private key
RSA *stringToRSAPrivateKey(const std::string &privateKeyString)
{
    BIO *bio = BIO_new_mem_buf(privateKeyString.c_str(), -1);
    RSA *privateKey = PEM_read_bio_RSAPrivateKey(bio, nullptr, nullptr, nullptr);

    BIO_free(bio);

    return privateKey;
}

//this is for base64 encode/decoding.....
typedef unsigned char BYTE;

static const std::string base64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

bool is_base64(BYTE c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(BYTE const* buf, unsigned int bufLen) {
    std::string ret;
    int i = 0;
    int j = 0;
    BYTE char_array_3[3];
    BYTE char_array_4[4];

    while (bufLen--) {
        char_array_3[i++] = *(buf++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i < 4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';
    }

    return ret;
}

std::vector<BYTE> base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    BYTE char_array_4[4], char_array_3[3];
    std::vector<BYTE> ret;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
    }

    return ret;
}
void encodeFileToFile(const std::string& inputFilename, const std::string& outputFilename)
{
    std::ifstream inputFile(inputFilename, std::ios::binary);
    std::vector<BYTE> buffer(std::istreambuf_iterator<char>(inputFile), {});

    std::string encoded = base64_encode(buffer.data(), buffer.size());

    std::ofstream outputFile(outputFilename);
    outputFile << encoded;
    outputFile.close();
}

void decodeFileToFile(const std::string& inputFilename, const std::string& outputFilename)
{
    std::ifstream inputFile(inputFilename);
    std::string encoded((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    std::vector<BYTE> decoded = base64_decode(encoded);

    std::ofstream outputFile(outputFilename, std::ios::binary);
    outputFile.write(reinterpret_cast<const char*>(decoded.data()), decoded.size());
    outputFile.close();
}
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

//     // Store Private Key into rsa_private_key.pem file
//     std::ofstream privateKeyData("../../keys/rsa_private_key.pem");
//     privateKeyData << rsaPublicKeyToString(privateKey);

//     // Read That Key
//     std::ifstream ifstream("../../keys/rsa_private_key.pem");
//     std::string someKey;
//     ifstream >> someKey;

//     // Cleanup
//     RSA_free(publicKey);
//     RSA_free(privateKey);

//     return 0;
// }
//---------------------------------------------------------------------------------------------------

#endif