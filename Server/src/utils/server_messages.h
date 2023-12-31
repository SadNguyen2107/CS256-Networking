#ifndef SERVER_MESSAGES_H
#define SERVER_MESSAGES_H

#include <boost/log/trivial.hpp>

//? For Logging Purpose
#define BOOST_LOG_DYN_LINK 1

//? FOR GENERAL MESSAGE
#define FAIL                    0x00000000
#define SUCCESS                 0x00000001

//? For CONNECTION MESSAGE
#define CONNECTION_FAIL         0x00000000
#define CONNECTION_SUCCESS      0x00000001
#define CONNECTION_DENY         0x00000002
#define CONNECTION_PERMIT       0x00000003
#define CLIENT_CONNECTION_CLOSE 0x00000004
#define SERVER_CONNECTION_CLOSE 0x00000005

//? FOR SEND DATA MESSAGE
#define SEND_FAIL              0x00000000
#define SEND_SUCCESS           0x00000001 

//? FOR RECEIVE DATA MESSAG
#define RECEIVE_FAIL            0x00000000
#define RECEIVE_SUCCESS         0x00000001

//? FOR ENCRYPT MESSAGE
#define ENCRYPT_FAIL            0x00000000
#define ENCRYPT_SUCCESS         0x00000001

//? FOR DECRYPT MESSAGE
#define DECRYPT_FAIL            0x00000000
#define DECRYPT_SUCCESS         0x00000001

//? FOR APPEND JSON FILE
#define APPEND_JSON_FAIL        0x00000000
#define APPEND_JSON_SUCCESS     0x00000001

#endif