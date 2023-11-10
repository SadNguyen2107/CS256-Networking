#include "./src/include/client_handler.hpp"
#include "./src/include/user_authentication.hpp"
#include <csignal>

int greeting(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket)
{
    std::string message = "Hello From Server...";
    int status = sendData(client_socket, message);

    return status;
}

int main(int argc, char const *argv[])
{
    // Create io_service for SERVER
    boost::asio::io_context io_context;

    // IP Address of the server
    std::string IPv4ServerStr = "127.0.0.1";
    unsigned short port = 6969;

    // Convert the IP Address string to an endpoint object
    boost::asio::ip::address_v4 serverIPv4Adddress = boost::asio::ip::address_v4::from_string(IPv4ServerStr);
    boost::asio::ip::tcp::endpoint server_endpoint(serverIPv4Adddress, port);

    // Listening for any new incomming connection
    boost::asio::ip::tcp::acceptor acceptor_server(
        io_context,
        server_endpoint);

    // Show a Log of SET UP SERVER
    BOOST_LOG_TRIVIAL(info) << "Sever Configuration..." << std::endl;
    BOOST_LOG_TRIVIAL(debug) << "Server Address: " << server_endpoint.address() << std::endl;
    BOOST_LOG_TRIVIAL(debug) << "Server Port Opening: " << server_endpoint.port() << std::endl;

    // Register the SIGINT signal handler for graceful shutdown
    // Asynchronous Function
    // Activate By Pressed Ctrl + C
    std::signal(SIGINT, &handleShutdownSignal);

    try
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
            if (isAccept == CONNECTION_DENY)
            {
                closeConnection(client_socket);
                continue;
            }

            // Create a new Thread to Handle the CLient
            std::thread handle_thread(&HandleClient, client_socket);
            handle_thread.detach();

            clientsConnections.insert(client_socket);
        }
    }
    catch (const std::exception &e)
    {
        BOOST_LOG_TRIVIAL(fatal) << "Exception in Server: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
