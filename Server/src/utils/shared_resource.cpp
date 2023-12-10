#include "shared_resource.h"

// To Concurrently shutdown
std::atomic<bool> isRunning = true;

// To Store All The Client Connections
std::set<std::shared_ptr<boost::asio::ip::tcp::socket>> clientsConnections;

// To Store The IO_Context And Acceptor Server Listener
std::shared_ptr<boost::asio::io_context> io_context;
std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_server; 