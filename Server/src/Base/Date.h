#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

class Date
{
private:
    unsigned short day = 0;
    unsigned short month = 0;
    unsigned short year = 9999;

public:
    // CONSTRUCTTORS
    Date(const unsigned short day = 0, const unsigned short month = 0, const unsigned short year = 9999);

    Date(const std::string date);

    // For Debug Purpose
    friend std::ostream &operator<<(std::ostream &os, const Date &date);

    // PROPERTIES
    unsigned short getDay() const;
    unsigned short getMonth() const;
    unsigned short getYear() const;

    friend std::ostream &operator<<(std::ostream &os, const Date *date);

    // OPERATOR
    bool operator==(const Date &otherDate);
    bool operator!=(const Date &otherDate);
    
    bool later(const Date *otherDate);
    bool sooner(const Date *otherDate);

    bool soonerOrEqual(const Date *otherDate);
};

std::ostream &operator<<(std::ostream &os, const Date &date);
std::ostream &operator<<(std::ostream &os, const Date *date);

bool validateThroughLib(std::string date);
bool validateWithStream(std::string date);
bool validate(unsigned short a, unsigned short b, unsigned short c);
bool validate_null(const unsigned short a, const unsigned short b);

// d1: deadline
// d2: submission date
std::string checkState(const Date *d1, const Date *d2);

#endif