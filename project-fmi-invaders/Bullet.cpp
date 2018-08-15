/**
	FMI OOP 2018 Final Project
	Bullet.cpp

	@author Alexander Dimitrov
	@version 1.4 03/07/18
*/



#include "Bullet.h"
#include "GameEngine.h"
#include "Explosion.h"

const short DEFAULT_SPEED = 2;
const short DEFAULT_DAMAGE = 4;
const unsigned short DEFAULT_BULLET_WIDTH = 1;
const unsigned short DEFAULT_BULLET_HEIGHT = 1;

const char* BULLET_MODEL[] = { "-" };

Bullet::Bullet(unsigned short x, unsigned short y,
	char direction, unsigned shooterId) :
	Object(x, y, DEFAULT_BULLET_WIDTH, DEFAULT_BULLET_HEIGHT, BULLET),
	direction(direction),
	shooterId(shooterId),
	speed(DEFAULT_SPEED),
	damage(DEFAULT_DAMAGE)
{
	model = BULLET_MODEL;
	if (direction == -1)	// if it's an enemy bullet (traveling left)
		color = C_RED;		// it's color is set to red
	else
		color = C_LT_GRAY;	// else it's light gray
}


Bullet::Bullet(unsigned short x, unsigned short y,
	char direction, unsigned shooterId,
	unsigned short damage) :
	Object(x, y, DEFAULT_BULLET_WIDTH, DEFAULT_BULLET_HEIGHT, BULLET),
	direction(direction),
	shooterId(shooterId),
	speed(DEFAULT_SPEED),
	damage(damage)
{
	model = BULLET_MODEL;
	if (direction == -1)	// if it's an enemy bullet (traveling left)
		color = C_RED;		// it's color is set to red
	else
		color = C_LT_GRAY;	// else it's light gray
}


Bullet::Bullet(std::ifstream& in) :
	Object(in, BULLET)
{
	in.read((char*)&shooterId, sizeof(shooterId));
	in.read((char*)&speed, sizeof(speed));
	in.read((char*)&damage, sizeof(damage));
	in.read((char*)&direction, sizeof(direction));
	model = BULLET_MODEL;
}


Bullet::~Bullet() {}


void Bullet::update()
{
	// Itterates through the places between where the bullet is now and where it will be in the next frame
	for (unsigned char i = 1; i <= speed; i++) {
		// If the bullet is inside the gameWindow
		if ((x + (direction * i)) >= 0 && (x + (direction * i)) < WindowHandler::i()->getGameWindowWidth()) {
			// If there is something in the place
			if (WindowHandler::i()->lookAt(x + direction * i, y) != ' ') {
				// Checks what's in that place
				Object* other = GameEngine::i()->getObjectAt(x + direction * i, y);
				if (other) {
					// If it's and enemy and the bullet was shot from the player
					// Or the other way around
					if ((other->getObjectType() == ENEMY && direction == 1) ||
						(other->getObjectType() == PLAYER && direction == -1))
					{
						// That object gets damaged
						other->receiveDamage(damage);
						// A small explosion is created on impact
						GameEngine::i()->instanciateObject(new Explosion(x + width / 2 + direction * i, y + height / 2, EXPLOSION_TINY));
						// The bullet gets destroyed
						GameEngine::i()->destroyObject(id);
						// A sound is being played
						PlaySound(TEXT("Sounds/bullet_hit.wav"), NULL, SND_FILENAME | SND_ASYNC);
						// Terminate the update function
						return;
					}
				}
			}
		}
	}

	// Otherwise

	// Move the bullet
	x += direction * speed;

	// And destroy it if it ends up outside the gameWindow
	if (x < 0 || x >= WindowHandler::i()->getGameWindowWidth() || y < 0 || y >= WindowHandler::i()->getGameWindowHeight()) {
		GameEngine::i()->destroyObject(id);
		return;
	}
}

void Bullet::serialize(std::ofstream& out) const
{
	Object::serialize(out);
	out.write((const char*)&shooterId, sizeof(shooterId));
	out.write((const char*)&speed, sizeof(speed));
	out.write((const char*)&damage, sizeof(damage));
	out.write((const char*)&direction, sizeof(direction));
}
