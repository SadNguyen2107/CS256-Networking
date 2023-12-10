#include "connect.h"

ConnectStatus connectSQLite(std::string path_to_db, sqlite3 **ppDB)
{
    int success = sqlite3_open_v2(path_to_db.c_str(), ppDB, SQLITE_OPEN_READWRITE, NULL);
    if (success != SQLITE_OK)
    {
        BOOST_LOG_TRIVIAL(error) << "Error open DB " << sqlite3_errmsg(*ppDB) << std::endl;
        return CONNECT_FAIL;
    }
    return CONNECT_SUCCESS;
}

CloseStatus closeSQLite(sqlite3 **ppDB)
{
    int success = sqlite3_close_v2(*ppDB);
    if (success != SQLITE_OK)
    {
        BOOST_LOG_TRIVIAL(error) << "Error closing database: " << sqlite3_errmsg(*ppDB) << std::endl;
        return CLOSE_FAIL;
    }

    // Change it back to nullptr
    *ppDB = nullptr;
    return CLOSE_SUCCESS;
}