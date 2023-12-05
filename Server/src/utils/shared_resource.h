#ifndef SHARED_RESOURCE_H
#define SHARED_RESOURCE_H

#include <boost/asio.hpp>
#include <memory>
#include <atomic>
#include <set>
#include <csignal>

// To Concurrently shutdown
static std::atomic<bool> isRunning = true;

// To Store All The Client Connections
std::set<std::shared_ptr<boost::asio::ip::tcp::socket>> clientsConnections;

// To Store The IO_Context And Acceptor Server Listener
std::shared_ptr<boost::asio::io_context> io_context;
std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_server; 

#endif