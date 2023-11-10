#ifndef CLIENT_HANDLER_HPP
#define CLIENT_HANDLER_HPP

#include "../utils/io_operations.h"

int HandleClient(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket)
{
    // Create a Lock To Prevent All Threads still running if Pressed exit
    // std::unique_lock<std::mutex> lock(gMutex);

    try
    {
        // Handle the client Connection
        boost::system::error_code error;

        // Keep the Server Continue Running Until PressExit
        int status = SUCCESS;
        while (status == SUCCESS)
        {
            std::string response;
            status = getData(client_socket, response);

            BOOST_LOG_TRIVIAL(debug) << response << std::endl;
            if (response == "exit")
            {
                closeConnection(client_socket);
                break;
            }
        }

        return CLIENT_CONNECTION_CLOSE;
    }
    catch (const boost::system::system_error &e)
    {
        // Handle boost::asio exceptions here
        BOOST_LOG_TRIVIAL(error) << "Exception in Client Handler: " << e.what() << std::endl;
        return FAIL;
    }
    catch (const std::exception &e)
    {
        // If Fail then Print in the Server Terminal
        BOOST_LOG_TRIVIAL(error) << "Exception in Client Handler: " << e.what() << std::endl;

        return FAIL;
    }
}

// To Gracefully Shutdown the Server 
void handleShutdownSignal(int signal)
{
    BOOST_LOG_TRIVIAL(info) << "Received signal " << signal << ". Initiating graceful shutdown." << std::endl;
    isRunning = false; // Set the flag to stop accepting new connections

    BOOST_LOG_TRIVIAL(info) << "Server is shutting down. Goodbye!" << std::endl;
    // Gracefully shutdown all the connections
    for (std::shared_ptr<boost::asio::ip::tcp::socket> client_socket : clientsConnections)
    {
        // Send a shutdown message and close the client socket
        sendData(client_socket, "exit");
    }

    // Exit the Program
    exit(1);
}


#endif