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
	virtual void serialize(std::ofstream& out) const;
	
	/**
		Creates a bullet infront of the enemy 
	*/
	void shoot();

protected:
	short hp;							// health points
	unsigned char fireRate;				// amount of ticks before enemy can shoot again
	unsigned char horizontalMoveRate;	// horizontal movement speed
	unsigned char verticalMoveRate;		// vertical movement speed
	unsigned char directionRate;		// amount of ticks before enemy can change direction
	char direction;						// either 1 or -1 (1 == right, -1 == left)
	unsigned char value;				// amount of points awarded to the player for destroying the enemy
};

#endif // !__ENEMY_HEADER_FILE_INCLUDED__
