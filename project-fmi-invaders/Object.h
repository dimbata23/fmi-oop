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
	Object(unsigned short x, unsigned short y, 
		   unsigned short width, unsigned short height, 
		   ObjectType type, int subType = -1);
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

	/**
		Returns the width of the object
	*/
	virtual unsigned getWidth() const;

	/**
		Returns the height of the object
	*/
	virtual unsigned getHeight() const;

	/**
		Returns the id of the object
	*/
	unsigned getId() const;

	/**
		Returns the x position of the object
	*/
	unsigned getX() const;

	/**
		Returns the y position of the object
	*/
	unsigned getY() const;

	/**
		Returns the type of the object
	*/
	ObjectType getObjectType() const;

	/**
		Damages the object with the specified damage

		@param damage the damage inflicted to the object.
		@return the actual damage taken
	*/
	virtual unsigned short receiveDamage(unsigned short damage);

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
	const char** model;		// Matrix model of the object
	unsigned id;			// ID
	const ObjectType type;	// Object's type
	const int subType;		// Object's subtype
	Color color;			// Color of the object
	unsigned short x;		// Object's x position
	unsigned short y;		// Object's y position
	unsigned short width;	// Object's width
	unsigned short height;	// Object's height
};

#endif // !__OBJECT_HEADER_INCLUDED__
