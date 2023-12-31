#include "client_handler.hpp"

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

                // Handle ID Send
                // HandleID
                HandleID(client_socket, &responseJSON);

                // Handle Client Key Send
                HandleKey(client_socket, &responseJSON);

                // Handle File Send
                HandleFile(client_socket, &responseJSON);
            }
            // If it is not a JSON object
            catch (const json::parse_error &e)
            {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
            catch (const std::exception &e)
            {
                // BOOST_LOG_TRIVIAL(debug) << responseStr << std::endl;
                std::cerr << "Error: " << e.what() << std::endl;
                std::cerr << "Error Contents: " << std::endl;
                std::cerr << responseStr << std::endl;

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

    // Gracefully shutdown all the connections
    for (std::shared_ptr<boost::asio::ip::tcp::socket> client_socket : clientsConnections)
    {
        BOOST_LOG_TRIVIAL(info) << "Shutting Connection with " << client_socket->remote_endpoint().address() << std::endl;
        // Send a shutdown message and close the client socket
        sendData(client_socket, "CLOSE BY SERVER");
        client_socket->close();
    }
    // Stop accepting new connections
    BOOST_LOG_TRIVIAL(info) << "Stopped accepting new connections." << std::endl;
    acceptor_server->close();

    // Stop the io_context to exit the run loop
    BOOST_LOG_TRIVIAL(info) << "Server is shutting down. Goodbye!" << std::endl;
    io_context->stop();
}

void HandleKey(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket, json *responseJSON)
{
    if (responseJSON->contains("key"))
    {
        // For Checking Receiving Key
        // BOOST_LOG_TRIVIAL(debug) << "Receive a PUBLIC KEY from " << client_socket->remote_endpoint().address();
        // BOOST_LOG_TRIVIAL(debug) << "Key: ";
        // BOOST_LOG_TRIVIAL(debug) << responseJSON.at("key") << std::endl;

        // Change the content of the key
        responseJSON->erase("id");  // Clear The Content Of the HustID
        responseJSON->emplace(client_socket->remote_endpoint().address().to_string(), responseJSON->at("key"));
        responseJSON->erase("key");

        // Store the client public key into the file
        std::string clients_keys_json_file = "keys/clients_public_key.json";
        append_to_JSON_file(clients_keys_json_file, responseJSON);
    }
}

void HandleFile(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket, json *responseJSON)
{
    // if (responseJSON->find("file") != responseJSON->end())
    // {
    //     auto list_of_file = responseJSON->at("file");
    //     for (auto it = list_of_file.begin(); it != list_of_file.end(); it++)
    //     {
    //         std::cout << "list of file from:" << client_socket->remote_endpoint().address().to_string() << std::endl;
    //         auto result = base64_decode(*it);
    //         for (size_t i = 0; i < result.size(); i++)
    //         {
    //             std::cout << result[i];
    //         }
    //         std::cout << std::endl;
    //     }
    // }

    if (responseJSON->contains("file"))
    {
        json fileData = responseJSON->at("file");

        for (auto it = fileData.begin(); it != fileData.end(); ++it)
        {
            // Extract filename
            std::string filename = it.key();
            std::cout << "Filename: " << filename << std::endl;

            // Extract file content
            std::string fileContent = it.value();
            std::cout << "File Content: " << fileContent << std::endl;

            // You can now use 'filename' and 'fileContent' as needed.
            // For example, you can write the content to a file.
            std::ofstream outputFile("hello.txt");
            outputFile << fileContent;
            outputFile.close();
        }
    }

    /*
    where to save the file: /data/name_of_project/group_id/file_name
    backup file to decode: /backup/name_of_project/group_id/file_name
    */
}

void HandleID(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket, json *responseJSON)
{
    // querying all the data
    // send to client
    //  Check For Hust-ID to send back
    if (responseJSON->contains("id"))
    {
        // Get Data From Database
        // 1st Step: Get groupID by calling getStudentInfo
        // 2nd Step: Get GroupInfo

        // Get Hust-ID From JSON file
        int id = responseJSON->at("id");

        // Get Student With the following id
        sqlite3 *db = nullptr;
        ConnectStatus ConnectStatus = connectSQLite("../Database/projects.db", &db);
        if (ConnectStatus == ConnectStatus::CONNECT_FAIL)
        {
            BOOST_LOG_TRIVIAL(error) << "Cannot Open The Database!" << std::endl;
            return;
        }

        int groupID = getStudentInfo(db, id, nullptr);
        if (groupID < 0)
        {
            BOOST_LOG_TRIVIAL(error) << "No group belong to!" << std::endl;
            closeSQLite(&db);
            return;
        }

        Group *group = nullptr;
        GroupStatus groupStatus = getGroupInfo(db, groupID, &group);
        if (groupStatus == GroupStatus::GROUP_NOT_EXIST)
        {
            closeSQLite(&db);
            return;
        }

        // Close DB

        // If Don't use Group Object anymore
        std::cout << "Group Info: " << std::endl;
        std::cout << group << std::endl;

        if (group != nullptr)
        {
            delete group;
        }

        closeSQLite(&db);
    }
}