#ifndef __ENEMY_BIRD_HEADER_INCLUDED__
#define __ENEMY_BIRD_HEADER_INCLUDED__

#include "Enemy.h"

class EnemyBird : public Enemy
{
public:
	EnemyBird(unsigned short x, unsigned short y);
	EnemyBird(std::ifstream&);
	~EnemyBird();
};

#endif // !__ENEMY_BIRD_HEADER_INCLUDED__
