#ifndef __ENEMY_HEADER_FILE_INCLUDED__
#define __ENEMY_HEADER_FILE_INCLUDED__

#include "Object.h"
#include "Bullet.h"

enum EnemySubType
{
	ENEMY_ARROW,
	ENEMY_BIRD,
	ENEMY_SPACESHIP
};

class Enemy : public Object
{
public:
	Enemy(unsigned short x, unsigned short y, EnemySubType subType);
	Enemy(std::ifstream&, EnemySubType subType);
	~Enemy();
	virtual void update();
	virtual unsigned getWidth() const;
	virtual unsigned getHeight() const;
	virtual unsigned char receiveDamage(unsigned char damage);
	void shoot();
	virtual void serialize(std::ofstream& out) const;
protected:
	short hp;
	unsigned char fireRate;
	unsigned char horizontalMoveRate;
	unsigned char verticalMoveRate;
	unsigned char directionRate;
	char direction;
	unsigned char value;
};

#endif // !__ENEMY_HEADER_FILE_INCLUDED__
