#include <iostream>
#include "System.h"
#include "CommandHandler.h"

const short MAX_BUFFER_SIZE = 1024;

int main()
{
    System sys;
    CommandHandler cmd;

    char buffer[MAX_BUFFER_SIZE];
    bool quit = false;
    while (!quit) {
        std::cout << "$: ";
        std::cin.getline(buffer, MAX_BUFFER_SIZE);
        quit = cmd(sys, buffer);
    }

    return 0;
}
