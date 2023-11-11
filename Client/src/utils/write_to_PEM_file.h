#ifndef WRITE_TO_PEM_FILE_H
#define WRITE_TO_PEM_FILE_H

#include "client_messages.h"
#include "encode_decode.h"
#include <fstream>

// Function to write the RSA public key to a file
int writeRSAPrivateKeyToPEMFile(RSA *privateKey, const std::string &filename)
{
    std::string pemString = rsaPublicKeyToString(privateKey);

    std::ofstream privateKeyFile(filename);
    if (!privateKeyFile.is_open())
    {
        BOOST_LOG_TRIVIAL(error) << "Unable to open file for writing." << std::endl;
        return WRITE_TO_FILE_FAIL;
    }

    privateKeyFile << pemString;
    privateKeyFile.close();
    BOOST_LOG_TRIVIAL(info) << "Public key successfully written to file: " << filename << std::endl;

    return WRITE_TO_FILE_SUCCESS;
}

#endif