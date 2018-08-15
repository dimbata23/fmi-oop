/**
	FMI OOP 2018 Final Project
	Enemy.cpp

	@author Alexander Dimitrov
	@version 1.4 03/07/18
*/



#include "Enemy.h"
#include "GameEngine.h"
#include "Explosion.h"

const short DEFAULT_HP = 8;
const unsigned char DEFAULT_FIRE_RATE = 30;
const unsigned char DEFAULT_MOVE_RATE = 2;
const unsigned char DEFAULT_DIRECTION_RATE = 16;
const unsigned char DEFAULT_VALUE = 10;
const unsigned short DEFAULT_WIDTH = 3;
const unsigned short DEFAULT_HEIGHT = 3;
const Color	DEFAULT_COLOR = C_RED;
const char* DEFAULT_MODEL[] = 
{
	"<--",
	"<--",
	"<--"
};


Enemy::Enemy(unsigned short x, unsigned short y, EnemySubType subType) :
	Object(x, y, DEFAULT_WIDTH, DEFAULT_HEIGHT, ENEMY, subType),
	hp(DEFAULT_HP),
	direction(1),
	fireRate(DEFAULT_FIRE_RATE),
	directionRate(DEFAULT_DIRECTION_RATE),
	horizontalMoveRate(DEFAULT_MOVE_RATE),
	verticalMoveRate(DEFAULT_MOVE_RATE),
	value(DEFAULT_VALUE)
{
	// If the enemy gets created too far down (because of the way enemies are created)
	// It gets moved back inside the gameWindow
	if (this->y + height >= WindowHandler::i()->getGameWindowHeight())
		this->y = WindowHandler::i()->getGameWindowHeight() - height;
	model = DEFAULT_MODEL;
	color = DEFAULT_COLOR;
}


Enemy::Enemy(std::ifstream& in, EnemySubType subType) :
	Object(in, ENEMY, subType)
{
	color = DEFAULT_COLOR;
	in.read((char*)&hp, sizeof(hp));
	in.read((char*)&fireRate, sizeof(fireRate));
	in.read((char*)&horizontalMoveRate, sizeof(horizontalMoveRate));
	in.read((char*)&verticalMoveRate, sizeof(verticalMoveRate));
	in.read((char*)&directionRate, sizeof(directionRate));
	in.read((char*)&direction, sizeof(direction));
	in.read((char*)&value, sizeof(value));
}


Enemy::~Enemy() 
{
	PlaySound(TEXT("Sounds/enemy_died.wav"), NULL, SND_FILENAME | SND_ASYNC);
}


void Enemy::update() 
{
	// Shoot if ready
	if (GameEngine::i()->getGameTick() % fireRate == 0)
		shoot();

	// Emulating some kind of Inteligence by aiming the enemy at the player's location
	if (GameEngine::i()->getGameTick() % directionRate == 0)
		if (GameEngine::i()->getPlayer()->getY() < y - height/2)
			direction = -1;
		else if (GameEngine::i()->getPlayer()->getY() + GameEngine::i()->getPlayer()->getHeight() - 1 > y + height/2)
			direction = 1;
		else 
			direction = 0;

	// Move horizontally if ready
	if (GameEngine::i()->getGameTick() % horizontalMoveRate == 0) 
		--x;
	
	// Move vertically if ready
	if (GameEngine::i()->getGameTick() % verticalMoveRate == 0)
		if (y + direction >= 0 && y + direction <= WindowHandler::i()->getGameWindowHeight() - height)
			y += direction;

	// Destroy the enemy if it ends up outside the gameWidnow
	if (x < 0 || x > WindowHandler::i()->getGameWindowWidth())
		GameEngine::i()->destroyObject(id);
}


unsigned Enemy::getWidth() const { return width; }
unsigned Enemy::getHeight() const { return height; }


unsigned char Enemy::receiveDamage(unsigned char damage) {
	// Take away health points
	this->hp -= damage;

	// If they end up below or equal to 0
	if (this->hp <= 0) {
		// Give points to the player
		GameEngine::i()->getPlayer()->receivePoints(this->value);
		// Create a medium explosion
		GameEngine::i()->instanciateObject(new Explosion(x + width / 2, y + height / 2));
		// Destroy the enemy object
		GameEngine::i()->destroyObject(this->id);
	}
	return damage;
}


void Enemy::shoot() 
{
	GameEngine::i()->instanciateObject(new Bullet(x - 1, y + height / 2, -1, id, 1 + rand() % 5));
}


void Enemy::serialize(std::ofstream& out) const 
{
	Object::serialize(out);
	out.write((const char*)&hp, sizeof(hp));
	out.write((const char*)&fireRate, sizeof(fireRate));
	out.write((const char*)&horizontalMoveRate, sizeof(horizontalMoveRate));
	out.write((const char*)&verticalMoveRate, sizeof(verticalMoveRate));
	out.write((const char*)&directionRate, sizeof(directionRate));
	out.write((const char*)&direction, sizeof(direction));
	out.write((const char*)&value, sizeof(value));
}
