#ifndef __OBJECT_HEADER_INCLUDED__
#define __OBJECT_HEADER_INCLUDED__

#include <fstream>
#include "WindowHandler.h"

enum ObjectType
{
	INVALID = -1,
	PLAYER,
	BULLET,
	ENEMY,
	EXPLOSION
};

class Object
{
public:
	Object();
	Object(unsigned short x, unsigned short y, unsigned short width, unsigned short height, ObjectType type, int subType = -1);
	Object(std::ifstream& in, ObjectType type, int subType = -1);
	virtual ~Object();
	virtual void update();
	virtual void drawSelf() const;
	virtual unsigned getWidth() const;
	virtual unsigned getHeight() const;
	unsigned getId() const;
	unsigned getX() const;
	unsigned getY() const;
	virtual unsigned char receiveDamage(unsigned char damage);
	ObjectType getObjectType() const;
	void virtual serialize(std::ofstream& out) const;
private:
	unsigned getNextId() const;
protected:
	const char** model;
	unsigned id;
	const ObjectType type;
	const int subType;
	Color color;
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
};

//extern Window window;

#endif // !__OBJECT_HEADER_INCLUDED__
