#ifndef __GAME_ENGINE_HEADER_INCLUDED__
#define __GAME_ENGINE_HEADER_INCLUDED__

#include "Object.h"
#include "Player.h"

class GameEngine {
public:
	// Get the GameEngine Instance
	static GameEngine* i();
	GameEngine(GameEngine const&) = delete;
	GameEngine& operator=(GameEngine const&) = delete;
	static const wchar_t gameName[];
	static const wchar_t* gameLogo[];
public:
	void initMainMenuHandles();
	Player* instanciatePlayer(Player*);
	Object* instanciateObject(Object*);
	void destroyObject(unsigned id);
	void updateAll();
	void drawAll() const;
	unsigned getNumOfObjects() const;
	unsigned getGameTick() const;
	Player* getPlayer() const;
	Object* getObjectAt(short x, short y, unsigned ignoreId = 0) const;
	void increaseGameSpeed();
	void decreaseGameSpeed();
	unsigned short getGameSpeed();
	void triggerGameOver();
	void triggerGameStop();
	bool isGameOver() const;
	bool saveFileExists() const;
	void destroySelf();
private:
	void openPauseMenu();
	void saveGame();
	void serialize(std::ofstream& out) const;
	void loadGame();
	Object* loadObject(std::ifstream& in) const;
private:
	static GameEngine* instance;
	Object** gameObjects;
	Player* playerObject;
	unsigned numOfObjects;
	unsigned capacity;
	unsigned gameTick;
	unsigned short gameSpeed;
	unsigned char spawnRate;
	bool gameOver;
	bool hasSaveFile;

	GameEngine();
	~GameEngine();
};

#endif // !__GAME_ENGINE_HEADER_INCLUDED__
