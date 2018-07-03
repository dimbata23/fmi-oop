#include <iostream>
#include <chrono>
#include <windows.h>
#include <mmsystem.h>
#include "WindowHandler.h"
#include "GameEngine.h"
#include "Object.h"
#include "Player.h"


int main() 
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WindowHandler::i()->initWindow();

	WindowHandler::i()->printGameLogo();

	WindowHandler::i()->initMainMenu();
	GameEngine::i()->initMainMenuHandles();

	std::chrono::steady_clock::time_point start_s;
	std::chrono::steady_clock::time_point stop_s;
	float calcTime;

	for (;;) {
		start_s = std::chrono::steady_clock::now();
		GameEngine::i()->updateAll();
		if (GameEngine::i()->isGameOver())
			break;
		WindowHandler::i()->clearGameWindow();
		GameEngine::i()->drawAll();
		WindowHandler::i()->drawStatus();
		WindowHandler::i()->drawGameWindow();
		stop_s = std::chrono::steady_clock::now();
		calcTime = std::chrono::duration <double, std::milli>(stop_s - start_s).count();
		WindowHandler::i()->drawInfo((((1000.0 / GameEngine::i()->getGameSpeed() - calcTime) <= 0) ? (1000 / calcTime) : (GameEngine::i()->getGameSpeed())), (short)calcTime);
		Sleep(max(1000.0 / GameEngine::i()->getGameSpeed() - calcTime, 0));
	}
	
	WindowHandler::i()->destroySelf();
	GameEngine::i()->destroySelf();

	return 0;
}