/**
	FMI OOP 2018 Final Project
	Object.cpp

	@author Alexander Dimitrov
	@version 1.41   03/07/18
*/



#include "Object.h"
#include "WindowHandler.h"


static unsigned currId = 1;		// starting from 1 so we have an invalid id of 0


Object::Object() : 
	model(nullptr), 
	x(0), 
	y(0), 
	width(0), 
	height(0), 
	id(getNextId()), 
	type(ObjectType(-1)), 
	subType(-1),
	color(C_WHITE)
{}


Object::Object(unsigned short x, unsigned short y, 
			   unsigned short width, unsigned short height, 
			   ObjectType type, int subType) :
	model(nullptr),
	x(x), 
	y(y), 
	id(getNextId()), 
	width(width), 
	height(height),
	type(type),
	subType(subType),
	color(C_WHITE)
{}


Object::Object(std::ifstream& in, ObjectType type, int subType) :
	type(type),
	model(nullptr),
	subType(subType),
	id(getNextId()),
	color(C_WHITE)
{
	in.read((char*)&x, sizeof(x));
	in.read((char*)&y, sizeof(y));
	in.read((char*)&width, sizeof(width));
	in.read((char*)&height, sizeof(height));
	in.read((char*)&color, sizeof(color));
}


Object::~Object() {}
void Object::update() {}
unsigned char Object::receiveDamage(unsigned char damage) { return damage; }


void Object::drawSelf() const 
{
	// Go through the model and add the characters to the gameWindow in their respective color
	for (unsigned char i = 0; i < height; ++i) {
		for (unsigned char j = 0; j < width; ++j) {
			if (model[i][j] != ' ') {
				WindowHandler::i()->setColorAt(x + j, y + i, color);
				WindowHandler::i()->writeAt(x + j, y + i, model[i][j]);
			}
		}
	}
}


unsigned Object::getWidth() const { return width; }
unsigned Object::getHeight() const { return height; }
unsigned Object::getId() const { return id; }
unsigned Object::getX() const { return x; }
unsigned Object::getY() const { return y; }
ObjectType Object::getObjectType() const { return type; }
unsigned Object::getNextId() const { return currId++; }


void Object::serialize(std::ofstream& out) const 
{
	// Standard serialization
	out.write((const char*)&type, sizeof(type));
	if (subType != -1)
		out.write((const char*)&subType, sizeof(subType));
	out.write((const char*)&x, sizeof(x));
	out.write((const char*)&y, sizeof(y));
	out.write((const char*)&width, sizeof(width));
	out.write((const char*)&height, sizeof(height));
	out.write((const char*)&color, sizeof(color));
}

