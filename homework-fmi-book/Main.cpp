#include <iostream>
#include "System.h"
#include "CommandHandler.h"

const short MAX_BUFFER_SIZE = 1024;

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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
