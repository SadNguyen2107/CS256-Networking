//? CONNECT TO SQLite Database Section
//? THIS FILE IS TO STORE ALL THE CONNECTION INFO

#ifndef CONNECTFUNC_H
#define CONNECTFUNC_H

#include "database_messages.h"

enum ConnectStatus {
    CONNECT_FAIL = 0,
    CONNECT_SUCCESS = 1
};

enum CloseStatus {
    CLOSE_FAIL = 0,
    CLOSE_SUCCESS = 1
};

// RETURN CONNECT STATUS
ConnectStatus connectSQLite(std::string path_to_db, sqlite3 **ppDb);
CloseStatus closeSQLite(sqlite3 **ppDb);

#endif