/**
    FMI OOP 2018 Final Project
    EnemySpaceship.cpp

    @author Alexander Dimitrov
    @version 1.41   03/07/18
*/



#include "EnemySpaceship.h"

const Color DEFAULT_SPACESHIP_COLOR = C_BLUE;
const char* SPACESHIP_MODEL[] =
{
    "<~ ",
    "<~~",
    "<~ "
};


EnemySpaceship::EnemySpaceship(unsigned short x, unsigned short y) : 
    Enemy(x, y, ENEMY_SPACESHIP)
{
    value *= 2.5;
    hp *= 1.25;
    fireRate *= 0.75;
    directionRate *= 0.75;
    width = 3;
    height = 3;
    model = SPACESHIP_MODEL;
    color = DEFAULT_SPACESHIP_COLOR;
}


EnemySpaceship::EnemySpaceship(std::ifstream& in) :
    Enemy(in, ENEMY_SPACESHIP)
{
    model = SPACESHIP_MODEL;
    color = DEFAULT_SPACESHIP_COLOR;
}


EnemySpaceship::~EnemySpaceship() {}
