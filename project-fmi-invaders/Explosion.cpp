/**
	FMI OOP 2018 Final Project
	Explosion.cpp

	@author Alexander Dimitrov
	@version 1.4 03/07/18
*/

#include "Explosion.h"
#include "GameEngine.h"

const Color DEFAULT_COLOR = C_LT_GRAY;

Explosion::Explosion(unsigned short x, unsigned short y) :
	Object(x, y, EXPLOSION_NORMAL, EXPLOSION_NORMAL, EXPLOSION),
	size(EXPLOSION_NORMAL),
	lifetime(0)
{}


Explosion::Explosion(unsigned short x, unsigned short y, ExplosionSize size) :
	Object(x, y, size, size, EXPLOSION),
	size(size),
	lifetime(0)
{}


Explosion::Explosion(std::ifstream& in) :
	Object(in, EXPLOSION)
{
	in.read((char*)&size, sizeof(size));
	in.read((char*)&lifetime, sizeof(lifetime));
}


Explosion::~Explosion() {}


void Explosion::update() 
{ 
	++lifetime;
	if (lifetime > size)
		GameEngine::i()->destroyObject(id);
}


void Explosion::drawSelf() const 
{
	for (unsigned i = 0; i < lifetime; ++i) {
		WindowHandler::i()->setColorAt(x - i, y, DEFAULT_COLOR);
		WindowHandler::i()->setColorAt(x + i, y, DEFAULT_COLOR);
		WindowHandler::i()->setColorAt(x - i, y - i, DEFAULT_COLOR);
		WindowHandler::i()->setColorAt(x + i, y + i, DEFAULT_COLOR);
		WindowHandler::i()->setColorAt(x - i, y + i, DEFAULT_COLOR);
		WindowHandler::i()->setColorAt(x + i, y - i, DEFAULT_COLOR);
		WindowHandler::i()->writeAt(x - i, y, '*');
		WindowHandler::i()->writeAt(x + i, y, '*');
		WindowHandler::i()->writeAt(x - i, y - i, '*');
		WindowHandler::i()->writeAt(x + i, y + i, '*');
		WindowHandler::i()->writeAt(x - i, y + i, '*');
		WindowHandler::i()->writeAt(x + i, y - i, '*');
	}
}


void Explosion::serialize(std::ofstream& out) const 
{
	Object::serialize(out);
	out.write((const char*)&size, sizeof(size));
	out.write((const char*)&lifetime, sizeof(lifetime));
}
