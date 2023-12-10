#include "accept_connections.hpp"

int greeting(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket)
{
    std::string message = "Hello From Server...";
    int status = sendData(client_socket, message);

    return status;
}

void handle_accept(const boost::system::error_code &error, std::shared_ptr<boost::asio::ip::tcp::socket> client_socket)
{
    if (!error)
    {
        // Connection accepted. Handle the connection using client_socket.
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
        if (isAccept == CONNECTION_DENY || isAccept == CLIENT_CONNECTION_CLOSE || !isRunning)
        {
            // SERVER DENY TO CONNECT
            closeConnection(client_socket);
            return;
        }

        // SERVER GRANT PERMISSION TO CONNECT
        sendData(client_socket, "OK TO CONNECT!");

        // Create a new Thread to Handle the CLient
        std::thread handle_thread(&HandleClient, client_socket);
        handle_thread.detach();

        clientsConnections.insert(client_socket);
    }
    else
    {
        // An error occurred.
        BOOST_LOG_TRIVIAL(error) << "Error accepting connection: " << error.message() << std::endl;
    }
}

void acceptConnections()
{
    // Connect the CLIENTS
    while (isRunning)
    {
        // Creating a socket to represent a connected client
        // Use Shared_ptr to share the owner ship instead of copying them
        std::shared_ptr<boost::asio::ip::tcp::socket> client_socket = std::make_shared<boost::asio::ip::tcp::socket>(*io_context);

        // Start an asynchronous accept operation
        acceptor_server->async_accept(*client_socket, std::bind(&handle_accept, std::placeholders::_1, client_socket));

        // Run the io_context to process asynchronous operations
        io_context->run();
    }
}