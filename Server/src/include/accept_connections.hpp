#ifndef ACCEPT_CONNECTION_HPP
#define ACCEPT_CONNECTION_HPP

#include "../utils/shared_resource.h"
#include "client_handler.hpp"
#include "user_authentication.hpp"


int greeting(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket)
{
    std::string message = "Hello From Server...";
    int status = sendData(client_socket, message);

    return status;
}

void acceptConnections(boost::asio::ip::tcp::acceptor& acceptor_server, boost::asio::io_context& io_context)
{
    // Connect the CLIENTS
    while (isRunning)
    {
        // Creating a socket to represent a connected client
        // Use Shared_ptr to share the owner ship instead of copying them
        std::shared_ptr<boost::asio::ip::tcp::socket> client_socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context);

        // Waiting for connection from Client
        acceptor_server.accept(*client_socket);

        // Get CLIENT'S IP Address and port
        boost::asio::ip::tcp::endpoint client_endpoint = client_socket->remote_endpoint();
        BOOST_LOG_TRIVIAL(debug) << "Connected To Client: "
                                 << client_endpoint.address()
                                 << ":"
                                 << client_endpoint.port()
                                 << std::endl;

        // Check If the Client has a Permission to Connect
        // Wait for the Client To Response
        greeting(client_socket);

        int isAccept = verifyClientConnection(client_socket);
        if (isAccept == CONNECTION_DENY || !isRunning)
        {
            // SERVER DENY TO CONNECT
            closeConnection(client_socket);
            continue;
        }

        // SERVER GRANT PERMISSION TO CONNECT
        sendData(client_socket, "OK TO CONNECT!");

        // Create a new Thread to Handle the CLient
        std::thread handle_thread(&HandleClient, client_socket);
        handle_thread.detach();

        clientsConnections.insert(client_socket);
    }
    BOOST_LOG_TRIVIAL(info) << "Stopped accepting new connections." << std::endl;
}

#endif