#ifndef CLIENT_H
#define CLIENT_H

// Network Header Files
#include "src/utils/client_messages.h"
#include "src/utils/shared_resource.h"
#include "src/utils/io_operations.h"
#include "src/utils/encode_decode.h"
#include "src/utils/write_to_PEM_file.h"
#include "src/utils/extract_data.h"
#include <thread>

void GenerateRSAkeys_and_Send(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket)
{
    // Generate RSA public key and RSA private key
    RSA *publicKey, *privateKey;
    privateKey = generateRSAKeyPair();
    publicKey = RSAPrivateKey_dup(privateKey);

    // Store Private Key into rsa_private_key.pem file
    writeRSAPrivateKeyToPEMFile(privateKey, "keys/rsa_private_key.pem");
    writeRSAPublicKeyToPEMFile(publicKey, "keys/rsa_public_key.pem");

    // Send the public Key to the Server
    json public_key_json_object;

    public_key_json_object.emplace("key", rsaPublicKeyToString(publicKey));

    // Free memory of the public and Private Key
    RSA_free(publicKey);
    RSA_free(privateKey);

    // Send To The Server
    sendData(client_socket, &public_key_json_object);
}

#endif