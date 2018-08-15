/**
	FMI OOP 2018 Final Project
	GameEngine.cpp

	@author Alexander Dimitrov
	@version 1.4 03/07/18
*/



#include <iostream>
#include <ctime>
#include <fstream>
#include "GameEngine.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "EnemySpaceship.h"
#include "EnemyBird.h"
#include "EnemyArrow.h"
#include "Explosion.h"

GameEngine* GameEngine::instance = nullptr;
const wchar_t GameEngine::gameName[] = L"Project \"FMI Invaders\" v1.4";

const wchar_t* GameEngine::gameLogo[] =
{
	L"         _____  __  __  _____                       ",
	L"        |  ___||  \\/  ||_   _|                      ",
	L"        | |_   | .  . |  | |                        ",
	L"        |  _|  | |\\/| |  | |                        ",
	L"        | |    | |  | | _| |_                       ",
	L"        \\_|    \\_|  |_/ \\___/                       ",
	L" _____                           _                  ",
	L"|_   _|                         | |                 ",
	L"  | |   _ __  __   __  __ _   __| |  ___  _ __  ___ ",
	L"  | |  | '_ \\ \\ \\ / / / _` | / _` | / _ \\| '__|/ __|",
	L" _| |_ | | | | \\ V / | (_| || (_| ||  __/| |   \\__ \\",
	L" \\___/ |_| |_|  \\_/   \\__,_| \\__,_| \\___||_|   |___/",
	nullptr
};

const char DEFAULT_SAVE_NAME[] = "last.sav";


// TODO: Multiple save files
// TODO: Допълнителни функционалности:
//		 Boss fights, след някакво време в играта, или когато се достигне определен резултат
//		 Бонус животи, които играчът може да събира, минавайки през тях.Те се появяват на екрана, през определен интервал
//		 Подобряване на оръжието на космическия кораб
//		 Различна траектория на снарядите
//		 Добавяне на “теренен пейзаж” - отгоре или отдолу игровото поле е ограничено от променящ се пейзаж - прелита се над сгради, скали и други препятствия.

const unsigned short MAX_SPEED = 120;
const unsigned short MIN_SPEED = 5;
const unsigned		 DEFAULT_CAPACITY = 16;
const unsigned short DEFAULT_GAME_SPEED = 12;
const unsigned char  DEFAULT_SPAWN_RATE = DEFAULT_GAME_SPEED * 4;
const unsigned char  DEFAULT_SPAWN_DIALATION = DEFAULT_GAME_SPEED * 2;
const unsigned char  DEFAULT_SPEED_INCREASE_RATE = DEFAULT_GAME_SPEED * 15;

const short KEY_UP     = 0x57;	// W
const short KEY_LEFT   = 0x41;	// A
const short KEY_DOWN   = 0x53;	// S
const short KEY_RIGHT  = 0x44;	// D


GameEngine::GameEngine() : 
	numOfObjects(0), 
	capacity(DEFAULT_CAPACITY), 
	gameObjects(nullptr), 
	gameTick(1), 
	gameSpeed(DEFAULT_GAME_SPEED),
	spawnRate(DEFAULT_SPAWN_RATE),
	gameOver(false)
{
	gameObjects = new Object*[capacity];
	srand(time(0));
	std::ifstream f(DEFAULT_SAVE_NAME);
	hasSaveFile = f.good();
}


GameEngine::~GameEngine() 
{
	for (unsigned i = 0; i < numOfObjects; ++i)
		delete gameObjects[i];
	delete[] gameObjects;
}


GameEngine* GameEngine::i() 
{
	if (!instance)
		instance = new GameEngine();
	return instance;
}


void GameEngine::initMainMenuHandles() 
{
	enum MainMenuOption
	{
		NEW_GAME,
		LOAD,
		QUIT,
		MM_OPTIONS_COUNT
	}option = NEW_GAME;

	// Draw the default slector position
	WindowHandler::i()->drawMainMenuSelector(option);

	// Trigger an infinte loop
	for (;;) {
		// If Enter was pressed
		if (GetAsyncKeyState(VK_RETURN)) {
			if (option == NEW_GAME) {
				break;
			} else if (option == LOAD) {
				loadGame();
				break;
			} else if (option == QUIT) {
				GameEngine::i()->triggerGameStop();
				return;
			}
		}

		// If KEY_UP was pressed (in this case 'W')
		if (GetAsyncKeyState(KEY_UP)) {
			// Play a sound
			PlaySound(TEXT("Sounds/menu_select.wav"), NULL, SND_FILENAME | SND_ASYNC);
			// Move the selector up
			if (option > 0)
				option = (MainMenuOption)(option - 1);
			if (option == LOAD && !hasSaveFile)
				option = NEW_GAME;
			// Draw the selector
			WindowHandler::i()->drawMainMenuSelector(option);
			// Wait 200 ms (Prevents moving the selector too fast)
			Sleep(200);
		} 
		// else If KEY_DOWN was pressed (in this case 'S')
		else if (GetAsyncKeyState(KEY_DOWN)) {
			// Play a sound
			PlaySound(TEXT("Sounds/menu_select.wav"), NULL, SND_FILENAME | SND_ASYNC);
			// Move the selector down
			if (option < MM_OPTIONS_COUNT - 1)
				option = (MainMenuOption)(option + 1);
			if (option == LOAD && !hasSaveFile)
				option = QUIT;
			// Draw the selector
			WindowHandler::i()->drawMainMenuSelector(option);
			// Wait 200 ms (Prevents moving the selector too fast)
			Sleep(200);
		}
	}

	// Once out of the loop

	// Clear the whole screen
	WindowHandler::i()->clearScreen();
	
	// Setup the game window 
	WindowHandler::i()->setupGameWindow();

	// If it's a new game create a new player object
	if (option == NEW_GAME)
		GameEngine::i()->instanciatePlayer(new Player);
}


Player* GameEngine::instanciatePlayer(Player* p) {
	return playerObject = (Player*)instanciateObject(p);
}


Object* GameEngine::instanciateObject(Object* o)
{
	// If there isn't enough capacity perform a standard resize
	// (Tip: It's good practice to seperate it in a different function)
	if (capacity <= numOfObjects) {
		unsigned newCapacity = capacity << 1;
		if (newCapacity > capacity) {
			Object** temp = new (std::nothrow) Object*[newCapacity];
			while (!temp) {
				--newCapacity;
				if (newCapacity > capacity) {
					temp = new (std::nothrow) Object*[newCapacity];
				} else {
					std::wcerr << "There wasn't enough memory to allocate for another object!\n";
					exit(-1);
				}
			}
			capacity = newCapacity;
			for (unsigned i = 0; i < numOfObjects; i++)
				temp[i] = gameObjects[i];
			delete[] gameObjects;
			gameObjects = temp;
		} else {
			std::wcerr << "Reached the limit of UINT!\n";
			exit(-2);
		}
	}

	// Save a pointer to the object
	gameObjects[numOfObjects] = o;
	// Increment the number of objects
	++numOfObjects;

	// Return the instansiated object
	return gameObjects[numOfObjects - 1];
}


void GameEngine::destroyObject(unsigned id)
{
	// Search for the object's index
	unsigned index = 0;
	for (unsigned i = 0; i < numOfObjects; ++i) {
		if (gameObjects[i]->getId() == id) {
			index = i;
			break;
		}
	}

	// Delete the object
	delete gameObjects[index];
	// Move the last object in it's place
	gameObjects[index] = gameObjects[numOfObjects - 1];
	// Decrease the number of objects
	--numOfObjects;
}

void GameEngine::updateAll()
{
	// Check for an Escape key and open the pause menu
	if (GetAsyncKeyState(VK_ESCAPE))
		openPauseMenu();

	// Run the update() function for all the objects
	for (unsigned i = 0; i < numOfObjects; ++i)
		gameObjects[i]->update();

	// Lausy enemy spawning
	if (gameTick % spawnRate == 0) {
		int xSpawn = WindowHandler::i()->getGameWindowWidth();
		int ySpawn = rand() % WindowHandler::i()->getGameWindowHeight();

		int percent = rand() % 101;
		if (percent <= 25)	// 25% chance of spawning a Bird
			instanciateObject(new EnemyBird(xSpawn, ySpawn));
		else if (percent > 25 && percent <= 40)	// 15% chance of spawning an Arrow
			instanciateObject(new EnemyArrow(xSpawn, ySpawn));
		else				// 60% chance of spawning a Spaceship
			instanciateObject(new EnemySpaceship(xSpawn, ySpawn));

		// Randomize the next spawn rate 
		spawnRate = DEFAULT_SPAWN_RATE + rand() % DEFAULT_SPAWN_DIALATION;
	}

	// Increase the speed of the game
	if (gameTick % DEFAULT_SPEED_INCREASE_RATE == 0)
		++gameSpeed;

	// .. counting gama ticks
	++gameTick;
}


void GameEngine::drawAll() const
{
	// Call the drawSelf() function for all the game objects
	for (unsigned i = 1; i < numOfObjects; ++i)
		gameObjects[i]->drawSelf();
	
	// Draw the player
	gameObjects[0]->drawSelf();
}


Object* GameEngine::getObjectAt(short x, short y, unsigned ignoreId) const {
	// Go through all objects and return the first one that
	// overlaps the position (x, y) and doesn't have id ignoreId
	for (unsigned i = 0; i < numOfObjects; ++i)
		if (x >= gameObjects[i]->getX() && x < gameObjects[i]->getX() + gameObjects[i]->getWidth() &&
			y >= gameObjects[i]->getY() && y < gameObjects[i]->getY() + gameObjects[i]->getHeight() &&
			gameObjects[i]->getId() != ignoreId)
		return gameObjects[i];
	return nullptr;
}

unsigned GameEngine::getNumOfObjects() const { return numOfObjects; }
unsigned GameEngine::getGameTick() const { return gameTick; }
Player* GameEngine::getPlayer() const { return playerObject; }
unsigned short GameEngine::getGameSpeed() { return gameSpeed; }

void GameEngine::increaseGameSpeed() { if (gameSpeed < MAX_SPEED) gameSpeed += 5; }
void GameEngine::decreaseGameSpeed() { if (gameSpeed > MIN_SPEED ) gameSpeed -= 5; }


void GameEngine::triggerGameOver() 
{
	WindowHandler::i()->drawStatus();
	WindowHandler::i()->drawGameOver();
	gameOver = true;
	// TODO: Loop until player has pressed escape to exit the game
	Sleep(3000);
}


void GameEngine::triggerGameStop() { gameOver = true; }
bool GameEngine::isGameOver() const { return gameOver; }
bool GameEngine::saveFileExists() const { return hasSaveFile; }


void GameEngine::destroySelf() 
{ 
	delete instance;
	instance = nullptr;
}


void GameEngine::openPauseMenu() 
{
	// Clear the place where the pause menu will be
	WindowHandler::i()->clearPauseMenu();
	// Draw the pause menu
	WindowHandler::i()->drawPauseMenu();
	// Draw the selector
	WindowHandler::i()->drawPauseMenuSelector(0);
	// Wait 200 ms (so that it doesn't detect the keypress that opened it)
	Sleep(200);

	enum PauseMenuOption
	{
		RESUME,
		SAVE,
		QUIT,
		PM_OPTIONS_COUNT
	}option = RESUME;

	for (;;) {
		// Break out of the loop if Escape is pressed
		if (GetAsyncKeyState(VK_ESCAPE))
			break;

		// Check for the Enter key
		if (GetAsyncKeyState(VK_RETURN)) {
			if (option == RESUME) {
				break;
			} else if (option == SAVE) {
				saveGame();
			} else if (option == QUIT) {
				GameEngine::i()->triggerGameStop();
				return;
			}
		}

		// If KEY_UP was pressed (in this case 'W')
		if (GetAsyncKeyState(KEY_UP)) {
			// Play a sound
			PlaySound(TEXT("Sounds/menu_select.wav"), NULL, SND_FILENAME | SND_ASYNC);
			// Move the selector up
			if (option > 0)
				option = (PauseMenuOption)(option - 1);
			// Draw the selector
			WindowHandler::i()->drawPauseMenuSelector(option);
			// Wait 200 ms (Prevents moving the selector too fast)
			Sleep(200);
		} else if (GetAsyncKeyState(KEY_DOWN)) {
			// Play a sound
			PlaySound(TEXT("Sounds/menu_select.wav"), NULL, SND_FILENAME | SND_ASYNC);
			// Move the selector down
			if (option < PM_OPTIONS_COUNT - 1)
				option = (PauseMenuOption)(option + 1);
			// Draw the selector
			WindowHandler::i()->drawPauseMenuSelector(option);
			// Wait 200 ms (Prevents moving the selector too fast)
			Sleep(200);
		}
		
	}

	// Once the pause menu is closed

	// Clear out the place where the pause menu was
	WindowHandler::i()->clearPauseMenu();
	// Wait 200 ms (so that it doesn't detect the keypress that closed it)
	Sleep(200);
}


void GameEngine::saveGame() 
{
	std::ofstream out(DEFAULT_SAVE_NAME, std::ios::binary | std::ios::trunc);
	serialize(out);
	out.close();
	WindowHandler::i()->showPauseMenuMessage("Game has been saved");
}


void GameEngine::serialize(std::ofstream& out) const 
{
	// Standard serialization
	out.write((const char*)&capacity, sizeof(capacity));
	out.write((const char*)&gameTick, sizeof(gameTick));
	out.write((const char*)&gameSpeed, sizeof(gameSpeed));
	out.write((const char*)&spawnRate, sizeof(spawnRate));

	out.write((const char*)&numOfObjects, sizeof(numOfObjects));
	for (unsigned i = 0; i < numOfObjects; ++i)
		gameObjects[i]->serialize(out);
}


void GameEngine::loadGame() 
{
	// Standard deserialization
	std::ifstream in(DEFAULT_SAVE_NAME, std::ios::binary);
	in.read((char*)&capacity, sizeof(capacity));
	in.read((char*)&gameTick, sizeof(gameTick));
	in.read((char*)&gameSpeed, sizeof(gameSpeed));
	in.read((char*)&spawnRate, sizeof(spawnRate));

	in.read((char*)&numOfObjects, sizeof(numOfObjects));
	delete[] gameObjects;
	gameObjects = new Object*[capacity];
	for (unsigned i = 0; i < numOfObjects; ++i)
		gameObjects[i] = loadObject(in);
	playerObject = (Player*)gameObjects[0];
	in.close();
}


Object* GameEngine::loadObject(std::ifstream& in) const 
{
	ObjectType type;
	// Read the first few bytes of the stream
	in.read((char*)&type, sizeof(type));
	// Decide what object it is, create it with the stream and return it
	switch (type) {
		case PLAYER:
			return new Player(in);
			break;
		case BULLET:
			return new Bullet(in);
			break;
		case ENEMY:
			int eType;
			in.read((char*)&eType, sizeof(eType));
			switch (eType) {
				case ENEMY_ARROW:
					return new EnemyArrow(in);
					break;
				case ENEMY_BIRD:
					return new EnemyBird(in);
					break;
				case ENEMY_SPACESHIP:
					return new EnemySpaceship(in);
					break;
			}
			break;
		case EXPLOSION:
			return new Explosion(in);
	}
	// If object with the read type was not found return nullptr
	return nullptr;
}
