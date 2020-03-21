/**
    FMI OOP 2018 Final Project
    Player.cpp

    @author Alexander Dimitrov
    @version 1.41   03/07/18
*/



#include "Player.h"
#include "GameEngine.h"
#include "Bullet.h"

const unsigned int   DEFAULT_X_POS  = 0;
const unsigned int   DEFAULT_Y_POS  = 0;
const unsigned char  DEFAULT_LIVES  = 3;
const unsigned char  DEFAULT_HEALTH = 16;
const unsigned short DEFAULT_WIDTH  = 3;
const unsigned short DEFAULT_HEIGHT = 3;

const short KEY_UP    = 0x57;    // W
const short KEY_LEFT  = 0x41;    // A
const short KEY_DOWN  = 0x53;    // S
const short KEY_RIGHT = 0x44;    // D

const unsigned char DEFAULT_FIRE_RATE = 6;
const unsigned char DEFAULT_DAMAGE_COLLISION_WITH_ENEMY = 100;

const char* PLAYER_MODEL[] =
{
    "\\- ",
    "=X>",
    "/- "
};


Player::Player() : 
    Object(DEFAULT_X_POS, DEFAULT_Y_POS, DEFAULT_WIDTH, DEFAULT_HEIGHT, PLAYER), 
    lives(DEFAULT_LIVES), 
    health(DEFAULT_HEALTH),
    currFireTick(0),
    score(0)
{
    this->x = 1;
    this->y = WindowHandler::i()->getGameWindowHeight() / 2 - this->height / 2;
    model = PLAYER_MODEL;
}


Player::Player(std::ifstream& in) :
    Object(in, PLAYER)
{
    in.read((char*)&score, sizeof(score));
    in.read((char*)&lives, sizeof(lives));
    in.read((char*)&health, sizeof(health));
    in.read((char*)&currFireTick, sizeof(currFireTick));
    model = PLAYER_MODEL;
}


Player::~Player() {}


unsigned Player::getWidth() const { return width; }
unsigned Player::getHeight() const { return height; }
unsigned int Player::getScore() const { return score; }
unsigned char  Player::getLives() const { return lives; }
unsigned char Player::getHealth() const { return health; }

void Player::checkInput()
{
    // Moving the player
    if (GetAsyncKeyState(KEY_RIGHT))
        if (x + width < WindowHandler::i()->getGameWindowWidth())
            ++x;

    if (GetAsyncKeyState(KEY_LEFT))
        if (x > 0)
            --x;

    if (GetAsyncKeyState(KEY_UP))
        if (y > 0)
            --y;

    if (GetAsyncKeyState(KEY_DOWN))
        if (y + height < WindowHandler::i()->getGameWindowHeight())
            ++y;

    // Shooting if it can
    if (GetAsyncKeyState(VK_SPACE))
        if (currFireTick == 0)
            shoot();

    // DEBUG (INCREASING GAME SPEED WITH SHIFT)
    if (GetAsyncKeyState(VK_LSHIFT))
        GameEngine::i()->increaseGameSpeed();

    // DEBUG (INCREASING GAME SPEED WITH CONTROL)
    if (GetAsyncKeyState(VK_LCONTROL))
        GameEngine::i()->decreaseGameSpeed();

}


void Player::checkCollisionWithEnemy() 
{
    // Go through the model's characters
    for (unsigned char i = 0; i < height; ++i) {
        for (unsigned char j = 0; j < width; ++j) {
            // Where there is a characters different than ' ' (of the model)
            if (model[i][j] != ' ') {
                // Get an object at that position, ignoring the player
                Object* o = GameEngine::i()->getObjectAt(x + j, y + i, id);
                if (o && o->getObjectType() == ENEMY) {
                    // If such object is found destroy it
                    GameEngine::i()->destroyObject(o->getId());
                    // And let the player receive damage
                    receiveDamage(DEFAULT_DAMAGE_COLLISION_WITH_ENEMY);
                }
            }
        }
    }
}


void Player::update()
{
    if (currFireTick > 0)
        --currFireTick;
    checkInput();
    checkCollisionWithEnemy();
}


unsigned char Player::receiveDamage(unsigned char damage) {
    // Play a sound
    PlaySound(TEXT("Sounds/player_hit.wav"), NULL, SND_FILENAME | SND_ASYNC);

    // If it won't kill the player
    if (health - damage > 0) {
        // Take damage
        health -= damage;
    } else {
        // If it will kill the player
        // and the player has enough time
        if (lives - 1 >= 0) {
            // Take lives
            --lives;
            // Reset health
            health = DEFAULT_HEALTH;
        } else {
            // If the player has no lives left
            health = 0;
            // Trigger the game over function
            GameEngine::i()->triggerGameOver();
        }
    }

    // return the taken damage
    return damage;
}


void Player::receivePoints(unsigned int points) { this->score += points; }


void Player::shoot() 
{
    // Play sound
    PlaySound(TEXT("Sounds/player_shoot.wav"), NULL, SND_FILENAME | SND_ASYNC);
    
    // Create a bullet infront of the player
    GameEngine::i()->instanciateObject(new Bullet(x + width, y + height / 2, 1, id));

    // Reset the fire tick
    currFireTick = DEFAULT_FIRE_RATE;
}


void Player::serialize(std::ofstream & out) const 
{
    // Standard serialization
    Object::serialize(out);
    out.write((const char*)&score, sizeof(score));
    out.write((const char*)&lives, sizeof(lives));
    out.write((const char*)&health, sizeof(health));
    out.write((const char*)&currFireTick, sizeof(currFireTick));
}
