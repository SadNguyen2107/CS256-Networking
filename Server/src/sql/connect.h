//? CONNECT TO SQLite Database Section
//? THIS FILE IS TO STORE ALL THE CONNECTION INFO

#ifndef CONNECTFUNC_H
#define CONNECTFUNC_H

enum ConnectStatus {
    CONNECT_FAIL = 0,
    CONNECT_SUCCESS = 1
};

enum CloseStatus {
    CLOSE_FAIL = 0,
    CLOSE_SUCCESS = 1
};


#include <sqlite3.h>
#include <string>
#include <iostream>

// RETURN CONNECT STATUS
ConnectStatus connectSQLite(std::string path_to_db, sqlite3 **ppDb);
CloseStatus closeSQLite(sqlite3 **ppDb);

ConnectStatus connectSQLite(std::string path_to_db, sqlite3 **ppDB)
{
    int success = sqlite3_open_v2(path_to_db.c_str(), ppDB, SQLITE_OPEN_READWRITE, NULL);
    if (success != SQLITE_OK)
    {
        std::cerr << "Error open DB " << sqlite3_errmsg(*ppDB) << std::endl;
        return CONNECT_FAIL;
    }
    return CONNECT_SUCCESS;
}

CloseStatus closeSQLite(sqlite3 **ppDB)
{
    int success = sqlite3_close_v2(*ppDB);
    if (success != SQLITE_OK)
    {
        std::cerr << "Error closing database: " << sqlite3_errmsg(*ppDB) << std::endl;
        return CLOSE_FAIL;
    }

    // Change it back to nullptr
    *ppDB = nullptr;
    return CLOSE_SUCCESS;
}
#endif