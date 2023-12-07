#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>

class Exception : public std::exception
{
private:
    std::string message;

public:
    Exception(const std::string &msg) : message(msg){};
    ~Exception() {};

    // Override the what() method to provide a description of the exception
    const char *what() const noexcept override
    {
        return message.c_str();
    }
};

#endif