#ifndef SHARED_RESOURCE_H
#define SHARED_RESOURCE_H

#include <boost/asio.hpp>
#include <memory>
#include <atomic>
#include <set>

// To Concurrently shutdown
static std::atomic<bool> isRunning = true;

// To Store All The Client Connections
std::set<std::shared_ptr<boost::asio::ip::tcp::socket>> clientsConnections;


#endif