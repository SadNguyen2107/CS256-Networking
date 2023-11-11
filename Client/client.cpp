#include "src/utils/client_messages.h"
#include "src/utils/shared_resource.h"
#include "src/utils/io_operations.h"
#include "src/utils/encode_decode.h"
#include "src/utils/write_to_PEM_file.h"
#include <iostream>
#include <fstream>
#include <thread>

int main()
{
    boost::asio::io_context io_context;
    std::shared_ptr<boost::asio::ip::tcp::socket> client_socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context);

    std::string serverIPv4AddressStr = "127.0.0.1";
    unsigned short port = 6969;

    try
    {
        boost::asio::ip::tcp::endpoint serverEndpoint(boost::asio::ip::address::from_string(serverIPv4AddressStr), port);
        client_socket->connect(serverEndpoint);

        // Read the initial message from the server
        std::string initialMessage;
        getData(client_socket, initialMessage);
        std::cout << "Received: " << initialMessage << std::endl;

        // Get user_email
        std::string user_email;
        std::cout << "Enter Email: ";
        std::getline(std::cin, user_email);
        sendData(client_socket, user_email);

        // Generate RSA public key and RSA private key
        RSA *publicKey, *privateKey;
        privateKey = generateRSAKeyPair();
        publicKey = RSAPrivateKey_dup(privateKey);

        // Store Private Key into rsa_private_key.pem file
        writeRSAPrivateKeyToPEMFile(publicKey, "keys/rsa_private_key.pem");

        // Send the public Key to the Server
        json public_key_json_object;

        public_key_json_object.emplace("key", rsaPublicKeyToString(publicKey));
        sendData(client_socket, &public_key_json_object);

        // WAIT TILL THE SERVER GRANT PERMISSION TO CONNECT
        std::cout << "WAIT FOR THE SERVER GRANT PERMISSION TO CONNECT..." << std::endl;
        std::string status;
        getData(client_socket, status);

        if (status == "exit")
        {
            isRunning = false;
        }

        // Start a thread to receive data from the server
        std::thread receive_thread([&client_socket]()
                                   {
            std::string response;
            while (isRunning)
            {
                int status = getData(client_socket, response);
                if (status == FAIL)
                {
                    break;
                }

                std::cout << std::endl 
                        << "From Server: " << response 
                        << std::endl;

                if (response == "CLOSE BY SERVER")
                {
                    isRunning = false;
                    sendData(client_socket, "CLOSE BY SERVER");
                    return;
                }
                else if (response == "exit")
                {
                    isRunning = false;
                    return;
                }
                
            } });

        while (isRunning)
        {
            std::string message;
            std::cout << "Enter a message to send (or 'exit' to quit): ";
            std::getline(std::cin, message);

            if (message == "exit")
            {
                sendData(client_socket, "exit");
                // Close the client_socket when done
                receive_thread.join();

                continue;
            }

            // Try to Encrypt with AES-128
            // In Production use RSA Algo - 2048 bit
            std::string encrypted_message;
            encryptRSA(reinterpret_cast<const unsigned char *>(message.c_str()), publicKey, encrypted_message);

            sendData(client_socket, encrypted_message);
        }

        // Free memory of the public and Private Key
        RSA_free(publicKey);
        RSA_free(privateKey);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
