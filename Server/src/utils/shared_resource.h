#ifndef SHARED_RESOURCE_H
#define SHARED_RESOURCE_H

#include <boost/asio.hpp>
#include <memory>
#include <atomic>
#include <set>
#include <csignal>

// To Concurrently shutdown
extern std::atomic<bool> isRunning;

// To Store All The Client Connections
extern std::set<std::shared_ptr<boost::asio::ip::tcp::socket>> clientsConnections;

// To Store The IO_Context And Acceptor Server Listener
extern std::shared_ptr<boost::asio::io_context> io_context;
extern std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_server; 

#endif