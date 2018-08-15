#ifndef __BULLET_HEADER_INCLUDED__
#define __BULLET_HEADER_INCLUDED__

#include "Object.h"

class Bullet : public Object
{
public:
	Bullet(unsigned short x, unsigned short y,
		short direction, unsigned shooterId);
	Bullet(unsigned short x, unsigned short y,
		short direction, unsigned shooterId, unsigned short damage);
	Bullet(std::ifstream&);
	~Bullet();
public:
	virtual void update();
	virtual void serialize(std::ofstream& out) const;
	//char getDirection() const;
	//unsigned char getDamage() const;
protected:
	unsigned shooterId;
	unsigned char speed;
	unsigned char damage;
	char direction;
};


#endif // !__BULLET_HEADER_INCLUDED__
