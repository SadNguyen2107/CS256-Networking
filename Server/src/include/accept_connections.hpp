#ifndef ACCEPT_CONNECTION_HPP
#define ACCEPT_CONNECTION_HPP

#include "../utils/shared_resource.h"
#include "client_handler.hpp"
#include "user_authentication.hpp"

int greeting(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket);

void handle_accept(const boost::system::error_code &error, std::shared_ptr<boost::asio::ip::tcp::socket> client_socket);

void acceptConnections();

#endif