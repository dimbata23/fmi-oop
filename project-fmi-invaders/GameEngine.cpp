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
//const char SAVES_FILE_NAME[] = "list.sav";

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

	WindowHandler::i()->drawMainMenuSelector(option);

	for (;;) {
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

		if (GetAsyncKeyState(KEY_UP)) {
			PlaySound(TEXT("Sounds/menu_select.wav"), NULL, SND_FILENAME | SND_ASYNC);
			if (option > 0)
				option = (MainMenuOption)(option - 1);
			if (option == LOAD && !hasSaveFile)
				option = NEW_GAME;
			WindowHandler::i()->drawMainMenuSelector(option);
			Sleep(200);
		} else if (GetAsyncKeyState(KEY_DOWN)) {
			PlaySound(TEXT("Sounds/menu_select.wav"), NULL, SND_FILENAME | SND_ASYNC);
			if (option < MM_OPTIONS_COUNT - 1)
				option = (MainMenuOption)(option + 1);
			if (option == LOAD && !hasSaveFile)
				option = QUIT;
			WindowHandler::i()->drawMainMenuSelector(option);
			Sleep(200);
		}
	}

	WindowHandler::i()->clearScreen();
	WindowHandler::i()->setupGameWindow();
	if (option == NEW_GAME)
		GameEngine::i()->instanciatePlayer(new Player);
}

Player* GameEngine::instanciatePlayer(Player* p) {
	return playerObject = (Player*)instanciateObject(p);
}

Object* GameEngine::instanciateObject(Object* o)
{
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

	gameObjects[numOfObjects] = o;
	++numOfObjects;

	return gameObjects[numOfObjects - 1];
}

void GameEngine::destroyObject(unsigned id)
{
	unsigned index = 0;
	for (unsigned i = 0; i < numOfObjects; ++i) {
		if (gameObjects[i]->getId() == id) {
			index = i;
			break;
		}
	}
	delete gameObjects[index];
	gameObjects[index] = gameObjects[numOfObjects - 1];
	--numOfObjects;
}

void GameEngine::updateAll()
{
	if (GetAsyncKeyState(VK_ESCAPE))
		openPauseMenu();

	for (unsigned i = 0; i < numOfObjects; ++i)
		gameObjects[i]->update();

	if (gameTick % spawnRate == 0) {
		int xSpawn = WindowHandler::i()->getGameWindowWidth();
		int ySpawn = rand() % WindowHandler::i()->getGameWindowHeight();

		int percent = rand() % 101;
		if (percent < 25)
			instanciateObject(new EnemyBird(xSpawn, ySpawn));
		else if (percent >= 25 && percent < 40)
			instanciateObject(new EnemyArrow(xSpawn, ySpawn));
		else
			instanciateObject(new EnemySpaceship(xSpawn, ySpawn));

		spawnRate = DEFAULT_SPAWN_RATE + rand() % DEFAULT_SPAWN_DIALATION;
	}

	if (gameTick % DEFAULT_SPEED_INCREASE_RATE == 0)
		++gameSpeed;

	++gameTick;
}

void GameEngine::drawAll() const
{
	for (unsigned i = 0; i < numOfObjects; ++i)
		gameObjects[i]->drawSelf();
	gameObjects[0]->drawSelf();
}

unsigned GameEngine::getNumOfObjects() const { return numOfObjects; }

unsigned GameEngine::getGameTick() const {
	return gameTick;
}

Player* GameEngine::getPlayer() const {
	return playerObject;
}

Object* GameEngine::getObjectAt(short x, short y, unsigned ignoreId) const {
	for (unsigned i = 0; i < numOfObjects; ++i)
		if (x >= gameObjects[i]->getX() && x < gameObjects[i]->getX() + gameObjects[i]->getWidth() &&
			y >= gameObjects[i]->getY() && y < gameObjects[i]->getY() + gameObjects[i]->getHeight() &&
			gameObjects[i]->getId() != ignoreId)
		return gameObjects[i];
	return nullptr;
}

void GameEngine::increaseGameSpeed() { if (gameSpeed < MAX_SPEED) gameSpeed += 5; }

void GameEngine::decreaseGameSpeed() { if (gameSpeed > MIN_SPEED ) gameSpeed -= 5; }

unsigned short GameEngine::getGameSpeed() { return gameSpeed; }

void GameEngine::triggerGameOver() 
{
	WindowHandler::i()->drawStatus();
	WindowHandler::i()->drawGameOver();
	gameOver = true;
	// TODO: Loop until player has pressed escape to exit the game?
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
	WindowHandler::i()->clearPauseMenu();
	WindowHandler::i()->drawPauseMenu();
	WindowHandler::i()->drawPauseMenuSelector(0);
	Sleep(200);

	enum PauseMenuOption
	{
		RESUME,
		SAVE,
		QUIT,
		PM_OPTIONS_COUNT
	}option = RESUME;

	for (;;) {
		if (GetAsyncKeyState(VK_ESCAPE))
			break;

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

		if (GetAsyncKeyState(KEY_UP)) {
			if (option > 0)
				option = (PauseMenuOption)(option - 1);
			WindowHandler::i()->drawPauseMenuSelector(option);
			Sleep(200);
		} else if (GetAsyncKeyState(KEY_DOWN)) {
			if (option < PM_OPTIONS_COUNT - 1)
				option = (PauseMenuOption)(option + 1);
			WindowHandler::i()->drawPauseMenuSelector(option);
			Sleep(200);
		}
		
	}

	WindowHandler::i()->clearPauseMenu();
	Sleep(100);
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
	Object* result;
	ObjectType type;
	in.read((char*)&type, sizeof(type));
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
	return nullptr;
}

//
//void GameEngine::saveGame() 
//{
//	unsigned long long temp = std::cin.tellg();
//	std::cin.seekg(std::ios::end);
//	unsigned long long temp2 = std::cin.tellg();
//	std::cin.ignore(temp2 - temp);
//	std::cin.clear();
//	clearPauseMenu();
//	drawSaveMenu();
//	Sleep(100);
//	
//	char saveName[256];
//	char readName[256];
//	std::cin.getline(saveName, '\n');
//
//	std::ifstream savesFile(SAVES_FILE_NAME);
//	bool saveExists = false;
//	while (!savesFile.eof()) {
//		savesFile.getline(readName, 256);
//		if (!stricmp(saveName, readName)) {
//			saveExists = true;
//			break;
//		}
//	}
//	if (!saveExists) {
//
//	}
//
//	openPauseMenu();
//}
