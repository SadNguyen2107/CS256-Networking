#include "Exception.h"

Exception::Exception(const std::string &msg) : message(msg){};
Exception::~Exception() {};

// Override the what() method to provide a description of the exception
const char * Exception::what() const noexcept 
{
    return message.c_str();
}