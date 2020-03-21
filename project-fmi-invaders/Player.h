#ifndef __PLAYER_HEADER_INCLUDED__
#define __PLAYER_HEADER_INCLUDED__

#include "WindowHandler.h"
#include "Object.h"

class Player : public Object {
public:
    Player();
    Player(std::ifstream&);
    virtual ~Player();

    /**
        Returns the current score of the Player
    */
    unsigned int getScore() const;

    /**
        Returns the current lives the Player has left
    */
    unsigned char getLives() const;

    /**
        Returns the current health points of the Player
    */
    unsigned char getHealth() const;

    virtual void update();
    virtual unsigned getWidth() const;
    virtual unsigned getHeight() const;
    virtual unsigned char receiveDamage(unsigned char damage);
    virtual void serialize(std::ofstream& out) const;

    /**
        Add points to the player's score
    */
    void receivePoints(unsigned int points);

    /**
        Creates a bullet infront of the Player
    */
    void shoot();


protected:
    /**
        Checking for input from the user
    */
    void checkInput();

    /**
        Checks and handles colision with an enemy
        (Tip: Spliting the checking and the handling in different functions is a better practice)
    */
    void checkCollisionWithEnemy();

    
    unsigned int score;        // Player's score
    char lives;                // Player's lives
    char health;            // Player's health points
    unsigned currFireTick;    // Amount of ticks left until the player can shoot again
};

#endif // !__PLAYER_HEADER_INCLUDED__
