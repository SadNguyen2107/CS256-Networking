#ifndef CLIENT_HANDLER_HPP
#define CLIENT_HANDLER_HPP

#include "../utils/io_operations.h"
#include "../utils/write_to_json_file.h"

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
            // Receive a String Object
            std::string responseStr;
            status = getData(client_socket, responseStr);

            // Try to Parse it into JSON Object
            try
            {
                json responseJSON = json::parse(responseStr);
                if (responseJSON.find("key") != responseJSON.end())
                {
                    BOOST_LOG_TRIVIAL(debug) << "Receive a PUBLIC KEY from " << client_socket->remote_endpoint().address();
                    BOOST_LOG_TRIVIAL(debug) << "Key: ";
                    BOOST_LOG_TRIVIAL(debug) << responseJSON.at("key") << std::endl;

                    // Change the content of the key
                    responseJSON[client_socket->remote_endpoint().address().to_string()] = responseJSON["key"];
                    responseJSON.erase("key");

                    // Store the client public key into the file
                    std::string clients_keys_json_file = "keys/clients_public_key.json";
                    append_to_JSON_file(clients_keys_json_file, &responseJSON);
                }
            }
            // If it is not a JSON object
            catch (const std::exception &e)
            {
                BOOST_LOG_TRIVIAL(debug) << responseStr << std::endl;

                if (responseStr == "CLOSE BY SERVER")
                {
                    closeConnection(client_socket);
                    return SERVER_CONNECTION_CLOSE;
                } 
                else if (responseStr == "exit")
                {
                    sendData(client_socket, "exit");
                    closeConnection(client_socket);
                    return CLIENT_CONNECTION_CLOSE;
                }
                
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
        sendData(client_socket, "CLOSE BY SERVER");
    }

}

#endif