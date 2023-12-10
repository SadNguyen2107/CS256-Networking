#ifndef USER_AUTHENTICATION_HPP
#define USER_AUTHENTICATION_HPP

//-------------------------------------------------------------------------
// 1. Use Gmail Service for verify User Email (Optional)
// 2. Use Database to check the presence of the account
// 3. Create New User = use GMAIL to send OTP and user input OTP code here
//-------------------------------------------------------------------------

#include "../utils/io_operations.h"
#include <iostream>

int verifyClientConnection(std::shared_ptr<boost::asio::ip::tcp::socket> client_socket);

#endif