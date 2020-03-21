#ifndef __GAME_ENGINE_HEADER_INCLUDED__
#define __GAME_ENGINE_HEADER_INCLUDED__

#include "Object.h"
#include "Player.h"

class GameEngine {
public:
    /**
        Get the GameEngine instance
    */
    static GameEngine* i();
    GameEngine(GameEngine const&) = delete;
    GameEngine& operator=(GameEngine const&) = delete;
    static const wchar_t gameName[];
    static const wchar_t* gameLogo[];

    /**
        Initializes the "handlers" for the main menu
    */
    void initMainMenuHandles();

    /**
        Saves the given pointer as a Player object and returns it
    */
    Player* instanciatePlayer(Player* player);

    /**
        Saves the given pointer as a game object and returns it
    */
    Object* instanciateObject(Object*);

    /**
        Deletes a game object with the given id
    */
    void destroyObject(unsigned id);

    /**
        Runs the update() function for all game objects
    */
    void updateAll();

    /**
        Runs the drawSelf() function for all game objects
    */
    void drawAll() const;

    /**
        Returns the number of gameObjects
    */
    unsigned getNumOfObjects() const;

    /**
        Returns the number of game ticks passed since the start of the game
    */
    unsigned getGameTick() const;

    /**
        Returns a pointer to the Player object
    */
    Player* getPlayer() const;

    /**
        Returns a pointer to the object at position x and y ignoring object with id ignoreId
        Returns nullptr if no such object is found
    */
    Object* getObjectAt(short x, short y, unsigned ignoreId = 0) const;

    /**
        Increases the targeted speed (ticks) at which the game should run by 5
    */
    void increaseGameSpeed();

    /**
        Decreases the targeted speed (ticks) at which the game should run by 5
    */
    void decreaseGameSpeed();

    /**
        Returns the targeted speed (ticks) at which the game should run
    */
    unsigned short getGameSpeed();

    /**
        Triggers the game over event
    */
    void triggerGameOver();

    /**
        Triggers the game stop event
    */
    void triggerGameStop();

    /**
        Returns wheter the game has ended
    */
    bool isGameOver() const;

    /**
        Returns wheter there is a save file
    */
    bool saveFileExists() const;

    /**
        Destroys the GameEngine instance
    */
    void destroySelf();


private:
    /**
        Pauses the game and opens the pause menu
    */
    void openPauseMenu();

    /**
        Saves the game
    */
    void saveGame();

    /**
        Saves the information for the game in the given file stream
    */
    void serialize(std::ofstream& out) const;

    /**
        Loads the last saved game
    */
    void loadGame();

    /**
        Reads file stream and creates a game object from the info in the stream
    */
    Object* loadObject(std::ifstream& in) const;


private:
    static GameEngine* instance;    // GameEngine instance
    Object** gameObjects;            // Array of pointers to the game objects
    Player* playerObject;            // Pointer to the Player object
    unsigned numOfObjects;            // The number of game objects
    unsigned capacity;                // Capacity of the gameObjects array
    unsigned gameTick;                // Current game tick
    unsigned short gameSpeed;        // Current game speed
    unsigned char spawnRate;        // Current spawn rate of enemies
    bool gameOver;                    // The game has ended
    bool hasSaveFile;                // Save file exists

    GameEngine();
    ~GameEngine();
};

#endif // !__GAME_ENGINE_HEADER_INCLUDED__
