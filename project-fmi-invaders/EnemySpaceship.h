#ifndef __ENEMY_SPACESHIP_HEADER_INCLUDED__
#define __ENEMY_SPACESHIP_HEADER_INCLUDED__

#include "Enemy.h"

class EnemySpaceship : public Enemy
{
public:
    EnemySpaceship(unsigned short x, unsigned short y);
    EnemySpaceship(std::ifstream&);
    ~EnemySpaceship();
};

#endif // !__ENEMY_SPACESHIP_HEADER_INCLUDED__
