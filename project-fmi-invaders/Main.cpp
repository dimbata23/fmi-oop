/**
	FMI OOP 2018 Final Project
	Main.cpp

	@author Alexander Dimitrov
	@version 1.41   03/07/18
*/



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
	// Initiate and setup the Console window
	WindowHandler::i()->initWindow();

	// Draw the game's logo
	WindowHandler::i()->printGameLogo();

	// Initiate and setup the Main Menu
	WindowHandler::i()->initMainMenu();

	// And it's "handles"
	GameEngine::i()->initMainMenuHandles();


	// Used for displaying how much time was needed to calculate and draw each frame
	std::chrono::steady_clock::time_point start_s;
	std::chrono::steady_clock::time_point stop_s;
	float calcTime;

	// Game loop
	for (;;) {
		// Start counting time
		start_s = std::chrono::steady_clock::now();

		// Update all the objects
		GameEngine::i()->updateAll();

		// If the game has ended break out of the loop
		if (GameEngine::i()->isGameOver())
			break;

		// Clear the game window
		WindowHandler::i()->clearGameWindow();

		// Add all the objects' draw models to the frame
		GameEngine::i()->drawAll();

		// Draw the player's score/lives/health
		//    [this doesn't need to be drawn every frame]
		//    [only after an updated of the status has been detected]
		//    [.. this functionality hasn't been implemented]
		WindowHandler::i()->drawStatus();

		// Draw the frame
		WindowHandler::i()->drawGameWindow();

		// Stop counting time
		stop_s = std::chrono::steady_clock::now();
		calcTime = std::chrono::duration <double, std::milli>(stop_s - start_s).count();

		// Draw info about the game speed and calculation time
		WindowHandler::i()->drawInfo((((1000.0 / GameEngine::i()->getGameSpeed() - calcTime) <= 0) ? (1000 / calcTime) : (GameEngine::i()->getGameSpeed())), (short)calcTime);
		
		// Pause the application for exactly 1000 / (gameSpeed - calcTime) milisec.
		// i.e. it has an effect of a constant frame rate (unless calcTime exceeds the gameSpeed)
		Sleep(max(1000.0 / GameEngine::i()->getGameSpeed() - calcTime, 0));
	}
	

	// Before exiting the program, make sure to delete all the memory used
	WindowHandler::i()->destroySelf();
	GameEngine::i()->destroySelf();

	return 0;
}