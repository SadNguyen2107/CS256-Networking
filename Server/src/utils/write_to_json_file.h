#ifndef WRITE_TO_JSON_FILE_H
#define WRITE_TO_JSON_FILE_H

#include "server_messages.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

int append_to_JSON_file(const std::string &fileName, json *json_obj)
{
    // Open the file in read-write mode
    std::fstream file(fileName, std::ios::in | std::ios::out);

    if (!file.is_open())
    {
        BOOST_LOG_TRIVIAL(error) << "Unable to open the file for reading and writing." << std::endl;
        return APPEND_JSON_FAIL;
    }

    json jsonObject;
    file >> jsonObject;

    // Merge 2 JSON object
    jsonObject.merge_patch(*json_obj);

    // Set the write position at the beginning of the file
    file.seekp(0);

    // Write the updated JSON to the file
    file << jsonObject.dump(4); // The '4' is for pretty printing
    file.close();

    BOOST_LOG_TRIVIAL(debug) << "File updated successfully." << std::endl;
    return APPEND_JSON_SUCCESS;
}

// int main(int argc, char const *argv[])
// {
//     json jsonObj;
//     jsonObj["Email"] = "Hanu";

//     append_to_JSON_file("../../keys/clients_public_keys.json", &jsonObj);
//     return 0;
// }

#endif