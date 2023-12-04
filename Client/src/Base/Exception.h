#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>

class Exception : public std::exception
{
private:
    std::string msg;

public:
    Exception(const char *message) : msg(message) {}
    ~Exception(){};

    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

#endif