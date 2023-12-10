#include "./src/include/client_handler.hpp"
#include "./src/include/user_authentication.hpp"
#include "./src/include/accept_connections.hpp"

int main(int argc, char const *argv[])
{
    // Create io_service for SERVER
    io_context = std::make_shared<boost::asio::io_context>();

    // IP Address of the server
    std::string IPv4ServerStr = "127.0.0.1";
    unsigned short port = 6969;

    // Convert the IP Address string to an endpoint object
    boost::asio::ip::address_v4 serverIPv4Adddress = boost::asio::ip::address_v4::from_string(IPv4ServerStr);
    boost::asio::ip::tcp::endpoint server_endpoint(serverIPv4Adddress, port);

    // Listening for any new incomming connection
    acceptor_server = std::make_shared<boost::asio::ip::tcp::acceptor>(
        *io_context,
        server_endpoint);

    // Show a Log of SET UP SERVER
    BOOST_LOG_TRIVIAL(info) << "Sever Configuration..." << std::endl;
    BOOST_LOG_TRIVIAL(debug) << "Server Address: " << server_endpoint.address() << std::endl;
    BOOST_LOG_TRIVIAL(debug) << "Server Port Opening: " << server_endpoint.port() << std::endl;

    // Register the SIGINT signal handler for graceful shutdown
    // Asynchronous Function
    // Activate By Pressed Ctrl + C
    std::signal(SIGINT, &handleShutdownSignal);

    std::thread acceptThread(&acceptConnections);

    acceptThread.join();
    BOOST_LOG_TRIVIAL(info) << "Server shutting downs." << std::endl;
    return 0;
}
