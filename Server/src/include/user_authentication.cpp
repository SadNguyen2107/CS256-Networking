#include "user_authentication.hpp"

int verifyClientConnection(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket)
{
    std::string response;
    int status = getData(client_socket, response);
    if (status == FAIL)
    {
        sendData(client_socket, "exit");
        client_socket->close();
    }

    std::cout << "HUST-ID: " << response << " want to connect!" << std::endl;
    std::cout << "Permit: y or n? ";
    std::string server_response;
    std::getline(std::cin, server_response);
    if (server_response == "n")
    {
        return CONNECTION_DENY;
    }

    return CONNECTION_PERMIT;
}