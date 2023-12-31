#include "Date.h"

// Date Class Constructor
Date::Date(const unsigned short day, const unsigned short month, const unsigned short year)
{
    if (validate(day, month, year))
    {
        this->day = day;
        this->month = month;
        this->year = year;
        return;
    }
    else
    {
        this->day = 0;
        this->month = 0;
        this->year = 9999;
        return;
    }
}

// Date Class Constructor
Date::Date(const std::string date)
{
    unsigned day = 0, month = 0, year = 9999;
    if (validateThroughLib(date) && validateWithStream(date))
    {
        std::stringstream ss(date);
        char delimiter;
        ss >> day >> delimiter >> month >> delimiter >> year;
        this->day = day;
        this->month = month;
        this->year = year;
        return;
    }
    this->day = day;
    this->month = month;
    this->year = year;
}

// Properties
unsigned short Date::getDay() const
{
    return this->day;
}

unsigned short Date::getMonth() const
{
    return this->month;
}

unsigned short Date::getYear() const
{
    return this->year;
}

// Override Operator
bool Date::operator==(const Date &otherDate)
{
    return this->day == otherDate.day && this->month == otherDate.month && this->year == otherDate.year;
}
bool Date::operator!=(const Date &otherDate)
{
    return this->day != otherDate.day || this->month == otherDate.month || this->year == otherDate.year;
}

// Method For Checking
/*
    d1.later(d2): bool, check whether d1 is later than d2.
    ex:
        Date d1 = Date("17/10/2003");
        Date d2 = Date("18/10/2003");
        if (d1.later(d2)){
            cout <<"d1 is later than d2"<<endl;
        } else{
            cout<<"d1 is not later than d2"<<endl;
        }
    result: "d1 is not later than d2"

*/
bool Date::later(const Date *otherDate)
{
    if (this->year > otherDate->year)
    {
        return true;
    }
    else if (this->year == otherDate->year)
    {
        if (this->month > otherDate->month)
        {
            return true;
        }
        else if (this->month == otherDate->month)
        {
            if (this->day > otherDate->day)
            {
                return true;
            }
            return false;
        }
        return false;
    }
    return false;
}

// Sooner
/*
    d1.sooner(d2): bool, check whether d1 is sooner than d2.
ex:
    Date d1 = Date("17/10/2003");
    Date d2 = Date("18/10/2003");
    if (d1.sooner(d2)){
        cout <<"d1 is sooner than d2"<<endl;
    } else{
        cout<<"d1 is not sooner than d2"<<endl;
    }
result: "d1 is sooner than d2"

*/
bool Date::sooner(const Date *otherDate)
{
    if (this->year < otherDate->year)
    {
        return true;
    }
    else if (this->year == otherDate->year)
    {
        if (this->month < otherDate->month)
        {
            return true;
        }
        else if (this->month == otherDate->month)
        {
            if (this->day < otherDate->day)
            {
                return true;
            }
            return false;
        }
        return false;
    }
    return false;
}

bool Date::soonerOrEqual(const Date *otherDate)
{
    if (this->year <= otherDate->year)
    {
        return true;
    }
    else if (this->year == otherDate->year)
    {
        if (this->month <= otherDate->month)
        {
            return true;
        }
        else if (this->month == otherDate->month)
        {
            if (this->day <= otherDate->day)
            {
                return true;
            }
            return false;
        }
        return false;
    }
    return false;
}

// For Debug Purpose
std::ostream &operator<<(std::ostream &os, const Date &date)
{
    os << date.day << "/" << date.month << "/" << date.year;
    return os;
}

std::ostream &operator<<(std::ostream &os, const Date *date)
{
    if (date != nullptr)
    {
        os << date->day << "/" << date->month << "/" << date->year;
    }
    else
        os << "Unknown";

    return os;
}

bool validateThroughLib(const std::string date)
{
    std::stringstream ss(date);
    tm time{};
    ss >> std::get_time(&time, "%d/%m/%y");
    if (ss.fail())
    {
        std::cout << "Failed to parse date and time\n";
        return false;
    }
    return true;
}
bool validateWithStream(const std::string date)
{
    std::stringstream ss(date);
    unsigned short day, month, year;
    char delimiter;
    ss >> day >> delimiter >> month >> delimiter >> year;
    if ((month == 2) && (day >= 30))
    {
        return false;
    }
    return true;
}
bool validate(const unsigned short a, const unsigned short b, const unsigned short c)
{
    if (a < 0 || b < 0 || c < 0 || a >= 32 || b >= 13)
    {
        return false;
    }
    if (b == 2 && a >= 30)
    {
        return false;
    }
    return true;
}

// check as null day :))
bool validate_null(const unsigned short a, const unsigned short b)
{
    if (a == 0 || b == 0 || b > 13)
    {
        return true;
    }
    return false;
}

// d1: deadline
// d2: submission date
std::string checkState(const Date *d1, const Date *d2)
{
    // Get the current time
    std::time_t currentTime = std::time(nullptr);

    // Convert the current time to a tm struct
    std::tm *now = std::localtime(&currentTime);

    // Get today year, month, day
    int today_day = now->tm_mday;
    int today_month = (now->tm_mon + 1); // Month is 0-based
    int today_year = (now->tm_year + 1900);

    Date today = Date(today_day, today_month, today_year);

    if (d2 == nullptr)
    {
        if (!today.later(d1))
        {
            return "NULL";
        }
        else
        {
            return "Late";
        }
    }
    else
    {
        Date submission(d2->getDay(), d2->getMonth(), d2->getYear());
        if (!submission.later(d1))
        {
            return "On time";
        }
        else
        {
            return "Late";
        }
    }
}