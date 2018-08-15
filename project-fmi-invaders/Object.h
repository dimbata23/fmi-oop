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

	/**
		Code ran every frame
	*/
	virtual void update();
	/**
		Code for drawing the object
	*/
	virtual void drawSelf() const;

	virtual unsigned getWidth() const;
	virtual unsigned getHeight() const;
	unsigned getId() const;
	unsigned getX() const;
	unsigned getY() const;
	ObjectType getObjectType() const;

	/**
		Damages the object with the specified damage

		@param damage the damage inflicted to the object.
		@return the actual damage taken
	*/
	virtual unsigned char receiveDamage(unsigned char damage);

	/**
		Saves the object to a file with the specified stream

		@param out stream to be saved to.
	*/
	void virtual serialize(std::ofstream& out) const;


private:
	/**
		Returns the next available id
	*/
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

#endif // !__OBJECT_HEADER_INCLUDED__
