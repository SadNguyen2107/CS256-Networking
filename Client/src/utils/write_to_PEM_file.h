#ifndef WRITE_TO_PEM_FILE_H
#define WRITE_TO_PEM_FILE_H

#include "client_messages.h"
#include "encode_decode.h"
#include <fstream>

// Function to write the RSA public key to a file
int writeRSAPublicKeyToPEMFile(RSA *publicKey, const std::string &filename)
{
    std::string pemString = rsaPublicKeyToString(publicKey);

    std::ofstream publicKeyFile(filename);
    if (!publicKeyFile.is_open())
    {
        BOOST_LOG_TRIVIAL(error) << "Unable to open file for writing." << std::endl;
        return WRITE_TO_FILE_FAIL;
    }

    publicKeyFile << pemString;
    publicKeyFile.close();
    BOOST_LOG_TRIVIAL(info) << "Public key successfully written to file: " << filename << std::endl;

    return WRITE_TO_FILE_SUCCESS;
}

// Function to write the RSA public key to a file
int writeRSAPrivateKeyToPEMFile(RSA *privateKey, const std::string &filename)
{
    std::string pemString = rsaPrivateKeyToString(privateKey);

    std::ofstream privateKeyFile(filename);
    if (!privateKeyFile.is_open())
    {
        BOOST_LOG_TRIVIAL(error) << "Unable to open file for writing." << std::endl;
        return WRITE_TO_FILE_FAIL;
    }

    privateKeyFile << pemString;
    privateKeyFile.close();
    BOOST_LOG_TRIVIAL(info) << "Private key successfully written to file: " << filename << std::endl;

    return WRITE_TO_FILE_SUCCESS;
}

#endif