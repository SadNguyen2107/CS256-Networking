#include "write_to_json_file.h"

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