#ifndef WRITE_TO_JSON_FILE_H
#define WRITE_TO_JSON_FILE_H

#include "server_messages.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

int append_to_JSON_file(const std::string &fileName, json *json_obj);

// int main(int argc, char const *argv[])
// {
//     json jsonObj;
//     jsonObj["Email"] = "Hanu";

//     append_to_JSON_file("../../keys/clients_public_keys.json", &jsonObj);
//     return 0;
// }

#endif