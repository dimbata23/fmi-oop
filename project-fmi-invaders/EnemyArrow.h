#ifndef __ENEMY_ARROW_HEADER_INCLUDED__
#define __ENEMY_ARROW_HEADER_INCLUDED__

#include "Enemy.h"

class EnemyArrow : public Enemy
{
public:
	EnemyArrow(unsigned short x, unsigned short y);
	EnemyArrow(std::ifstream&);
	~EnemyArrow();
};


#endif // !__ENEMY_ARROW_HEADER_INCLUDED__
