#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>

class Exception : public std::exception
{
private:
    std::string message;

public:
    Exception(const std::string &msg);
    ~Exception();

    const char *what() const noexcept override;
};

#endif