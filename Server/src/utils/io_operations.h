#ifndef IO_OPERATIONS_H
#define IO_OPERATIONS_H

#include "json.hpp"
#include "server_messages.h"
#include "shared_resource.h"

using json = nlohmann::json;

// Get Data From the client_socket
// Get back String Object
int getData(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket, std::string &result);

// Get Data From the client_socket
// Get back Json Object
int getData(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket, json* json_result);

// Send the Data To the Client
// Send a Simple String Object
int sendData(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket, const std::string &message);

// Send the Data To the Client
// Send a JSON Object
int sendData(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket, const json* json_obj);

// Close the Connection of the client_socket
int closeConnection(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket);

#endif