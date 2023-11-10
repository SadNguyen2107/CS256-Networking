#ifndef INDEX_H
#define INDEX_H

#include <iostream>

#define FAIL     0x00000000
#define SUCCEESS 0x00000001


int displayGreeting()
{
    // Display the menu
    std::cout << "Index Menu:" << std::endl;
    std::cout << "1. Option 1" << std::endl;
    std::cout << "2. Option 2" << std::endl;
    std::cout << "3. Option 3" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Enter your choice: ";

    return SUCCEESS;
}

int displayIndexMenu()
{
    // Display the greeting template
    std::cout << "Greeting Sir/Madam...." << std::endl;

    return SUCCEESS;
}

#endif