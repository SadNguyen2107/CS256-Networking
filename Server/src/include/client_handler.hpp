#ifndef CLIENT_HANDLER_HPP
#define CLIENT_HANDLER_HPP

#include "../utils/encode_decode.h"
#include "../utils/io_operations.h"
#include "../utils/write_to_json_file.h"

#include "../sql/retrieve.h"
#include "../sql/connect.h"

// void HandleFile();
void HandleKey(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket, json *responseJSON);
void HandleFile(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket, json *responseJSON);
void HandleID(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket, json *responseJSON);

int HandleClient(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket);

// To Gracefully Shutdown the Server
void handleShutdownSignal(int signal);

#endif
