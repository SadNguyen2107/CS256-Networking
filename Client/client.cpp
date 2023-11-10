#include "src/utils/client_messages.h"
#include "src/utils/shared_resource.h"
#include "src/utils/io_operations.h"
#include "src/utils/encode_decode.h"
#include <iostream>
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

        // Start a thread to receive data from the server
        std::thread receive_thread([&client_socket](){
            std::string response;
            while (isRunning)
            {
                int status = getData(client_socket, response);
                if (status == FAIL)
                {
                    break;
                }
                if (status == CLOSE_FROM_SERVER)
                {
                    // FOR GRACEFUL SHUTDOWN PURPOSE
                    // SEND BACK TO THE SERVER
                    sendData(client_socket, "exit");
                }
                

                std::cout << "From Server: " << response << std::endl;

                if (response == "exit")
                {
                    isRunning = false;
                }
                
            } });

        while (isRunning)
        {
            std::string message;
            std::cout << "Enter a message to send (or 'exit' to quit): ";
            std::getline(std::cin, message);

            if (message == "exit")
            {
                isRunning = false;
                break;
            }

            if (!isRunning)
            {
                break;
            }
            // Try to Encrypt with AES-128
            // In Production use RSA Algo - 2048 bit
            std::string encrypted_message;
            encryptAES(message, reinterpret_cast<const unsigned char*>("Hello World"), encrypted_message);

            sendData(client_socket, encrypted_message);
        }

        // Close the client_socket when done
        if (!isRunning)
        {
            receive_thread.join();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
