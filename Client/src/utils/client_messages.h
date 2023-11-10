#ifndef CLIENT_MESSAGES_H
#define CLIENT_MESSAGES_H

#include <boost/log/trivial.hpp>

//? For Logging Purpose
#define BOOST_LOG_DYN_LINK 1

//? FOR GENERAL MESSAGE
#define FAIL                    0x00000000
#define SUCCESS                 0x00000001

//? For CONNECTION MESSAGE
#define CONNECTION_FAIL         0x00000000
#define CONNECTION_SUCCESS      0x00000001
#define CLOSE_FROM_SERVER       0x00000002

//? FOR SEND DATA MESSAGE
#define SEND_FAIL               0x00000000
#define SEND_SUCCESS            0x00000001 

//? FOR RECEIVE DATA MESSAG
#define RECEIVE_FAIL            0x00000000
#define RECEIVE_SUCCESS         0x00000001

//? FOR ENCRYPT MESSAGE
#define ENCRYPT_FAIL            0x00000000
#define ENCRYPT_SUCCESS         0x00000001

//? FOR DECRYPT MESSAGE
#define DECRYPT_FAIL            0x00000000
#define DECRYPT_SUCCESS         0x00000001

#endif