#ifndef IO_OPERATIONS_H
#define IO_OPERATIONS_H

#include "json.hpp"
#include "client_messages.h"
#include "shared_resource.h"

using json = nlohmann::json;

// Get Data From the client_socket
// Get back String Object
int getData(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket, std::string &result)
{
    try
    {
        // Check if any error occur before get Data
        if (!client_socket->is_open())
        {
            BOOST_LOG_TRIVIAL(error) << "Socket is not open for sending data." << std::endl;
            return CONNECTION_FAIL;
        }

        //! Get the data
        boost::asio::streambuf readerBuffer;
        boost::system::error_code error;

        size_t bytes_transferred = boost::asio::read(*client_socket, readerBuffer, boost::asio::transfer_at_least(1), error);

        // CHECK IF ANY ERROR
        if (error == boost::asio::error::eof)
        {
            BOOST_LOG_TRIVIAL(error) << "Connection closed by client!" << std::endl;
            return CONNECTION_FAIL;
        }
        else if (error)
        {
            BOOST_LOG_TRIVIAL(error) << "Error reading from client: " << error.message() << std::endl;
            return RECEIVE_FAIL;
        }
        else if (bytes_transferred <= 0)
        {
            // No data was received.
            return RECEIVE_FAIL;
        }

        // Change the buffer type into std::string
        std::istream response_stream(&readerBuffer);
        std::getline(response_stream, result);

        return RECEIVE_SUCCESS;
    }
    catch (const boost::system::system_error &e)
    {
        // Handle boost::asio exceptions here
        BOOST_LOG_TRIVIAL(error) << "Exception in Get Data From Client: " << e.what() << std::endl;
        return FAIL;
    }
    catch (const std::exception &e)
    {
        // Display ERROR When something happens during client data reception
        BOOST_LOG_TRIVIAL(error) << "Exception in Get Data From Client: " << e.what() << std::endl;
        return FAIL;
    }
}

// Get Data From the client_socket
// Get back Json Object
int getData(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket, json* json_result)
{
    try
    {
        // Check if any error occur before get Data
        if (!client_socket->is_open())
        {
            BOOST_LOG_TRIVIAL(error) << "Socket is not open for sending data." << std::endl;
            return CONNECTION_FAIL;
        }

        //! Get the data
        boost::asio::streambuf readerBuffer;
        boost::system::error_code error;

        size_t bytes_transferred = boost::asio::read(*client_socket, readerBuffer, boost::asio::transfer_at_least(1), error);

        // CHECK IF ANY ERROR
        if (error == boost::asio::error::eof)
        {
            BOOST_LOG_TRIVIAL(error) << "Connection closed by client!" << std::endl;
            return CONNECTION_FAIL;
        }
        else if (error)
        {
            BOOST_LOG_TRIVIAL(error) << "Error reading from client: " << error.message() << std::endl;
            return RECEIVE_FAIL;
        }
        else if (bytes_transferred <= 0)
        {
            // No data was received.
            return RECEIVE_FAIL;
        }

        // Change the buffer type into std::string
        std::istream response_stream(&readerBuffer);
        std::string serializedData(std::istreambuf_iterator<char>(response_stream), {});

        // Deserialize the JSON data back to an object
        *json_result = json::parse(serializedData);

        return RECEIVE_SUCCESS;
    }
    catch (const boost::system::system_error &e)
    {
        // Handle boost::asio exceptions here
        BOOST_LOG_TRIVIAL(error) << "Exception in Get Data From Client: " << e.what() << std::endl;
        return FAIL;
    }
    catch (const std::exception &e)
    {
        // Display ERROR When something happens during client data reception
        BOOST_LOG_TRIVIAL(error) << "Exception in Get Data From Client: " << e.what() << std::endl;
        return FAIL;
    }
}

// Send the Data To the Client
// Send a Simple String Object
int sendData(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket, const std::string &message)
{
    try
    {
        // Check the connection
        if (!client_socket->is_open())
        {
            BOOST_LOG_TRIVIAL(error) << "Socket is not open for sending data." << std::endl;
            return CONNECTION_FAIL;
        }

        // Send Data Back to the Server
        boost::system::error_code error;

        std::size_t bytes_transferred = boost::asio::write(*client_socket, boost::asio::buffer(message), error);
        // CHECK IF ANY ERROR
        if (error == boost::asio::error::eof)
        {
            BOOST_LOG_TRIVIAL(error) << "Connection closed by client!" << std::endl;
            return CONNECTION_FAIL;
        }
        else if (error)
        {
            BOOST_LOG_TRIVIAL(error) << "Error writing to the socket: " << error.message() << std::endl;
            return SEND_FAIL;
        }
        else if (bytes_transferred != message.size())
        {
            BOOST_LOG_TRIVIAL(error) << "Not all data was transfered successfully!" << std::endl;
            return SEND_FAIL;
        }

        return SEND_SUCCESS;
    }
    catch (const boost::system::system_error &e)
    {
        // Handle boost::asio exceptions here
        BOOST_LOG_TRIVIAL(error) << "Exception in Sending Message to Client: " << e.what() << std::endl;
        return FAIL;
    }
    catch (const std::exception &e)
    {
        // Display Error when send failed
        BOOST_LOG_TRIVIAL(error) << "Exception in Sending Message to Client: " << e.what() << std::endl;
        return FAIL;
    }
}

// Send the Data To the Client
// Send a JSON Object
int sendData(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket, const json* json_obj)
{
    try
    {
        // Check the connection
        if (!client_socket->is_open())
        {
            BOOST_LOG_TRIVIAL(error) << "Socket is not open for sending data." << std::endl;
            return CONNECTION_FAIL;
        }

        // Send Json Object to the Client
        boost::system::error_code error;

        std::size_t bytes_transferred = boost::asio::write(*client_socket, boost::asio::buffer(json_obj->dump()), error);

        // CHECK IF ANY ERROR
        if (error == boost::asio::error::eof)
        {
            BOOST_LOG_TRIVIAL(error) << "Connection closed by client!" << std::endl;
            return CONNECTION_FAIL;
        }
        else if (error)
        {
            BOOST_LOG_TRIVIAL(error) << "Error writing to the socket: " << error.message() << std::endl;
            return SEND_FAIL;
        }
        else if (bytes_transferred != json_obj->dump().size())
        {
            BOOST_LOG_TRIVIAL(error) << "Not all data was transfered successfully!" << std::endl;
            return SEND_FAIL;
        }

        return SEND_SUCCESS;
    }
    catch (const boost::system::system_error &e)
    {
        // Handle boost::asio exceptions here
        BOOST_LOG_TRIVIAL(error) << "Exception in Sending Message to Client: " << e.what() << std::endl;
        return SEND_FAIL;
    }
    catch (const std::exception &e)
    {
        // Display Error when send failed
        BOOST_LOG_TRIVIAL(error) << "Exception in Sending Message to Client: " << e.what() << std::endl;
        return SEND_FAIL;
    }
}

#endif