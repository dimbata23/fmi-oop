/**
	FMI OOP 2018 Final Project
	Player.cpp

	@author Alexander Dimitrov
	@version 1.4 03/07/18
*/

#include "Player.h"
#include "GameEngine.h"
#include "Bullet.h"

const unsigned int   DEFAULT_X_POS  = 0;
const unsigned int   DEFAULT_Y_POS  = 0;
const unsigned char  DEFAULT_LIVES  = 3;
const unsigned char  DEFAULT_HEALTH = 16;
const unsigned short DEFAULT_WIDTH  = 3;
const unsigned short DEFAULT_HEIGHT = 3;

const short KEY_UP    = 0x57;	// W
const short KEY_LEFT  = 0x41;	// A
const short KEY_DOWN  = 0x53;	// S
const short KEY_RIGHT = 0x44;	// D

const unsigned char DEFAULT_FIRE_RATE = 6;
const unsigned char DEFAULT_DAMAGE_COLLISION_WITH_ENEMY = 100;

const char* PLAYER_MODEL[] =
{
	"\\- ",
	"=X>",
	"/- "
};

Player::Player() : 
	Object(DEFAULT_X_POS, DEFAULT_Y_POS, DEFAULT_WIDTH, DEFAULT_HEIGHT, PLAYER), 
	lives(DEFAULT_LIVES), 
	health(DEFAULT_HEALTH),
	currFireTick(0),
	score(0)
{
	this->x = 1;
	this->y = WindowHandler::i()->getGameWindowHeight() / 2 - this->height / 2;
	model = PLAYER_MODEL;
}

Player::Player(std::ifstream& in) :
	Object(in, PLAYER)
{
	in.read((char*)&score, sizeof(score));
	in.read((char*)&lives, sizeof(lives));
	in.read((char*)&health, sizeof(health));
	in.read((char*)&currFireTick, sizeof(currFireTick));
	model = PLAYER_MODEL;
}

Player::~Player() {}

unsigned int Player::getScore() const { return score; }

unsigned char  Player::getLives() const { return lives; }

unsigned char Player::getHealth() const { return health; }

void Player::checkInput()
{
	if (GetAsyncKeyState(KEY_RIGHT))
		if (x + width < WindowHandler::i()->getGameWindowWidth())
			++x;

	if (GetAsyncKeyState(KEY_LEFT))
		if (x > 0)
			--x;

	if (GetAsyncKeyState(KEY_UP))
		if (y > 0)
			--y;

	if (GetAsyncKeyState(KEY_DOWN))
		if (y + height < WindowHandler::i()->getGameWindowHeight())
			++y;

	if (GetAsyncKeyState(VK_SPACE))
		if (currFireTick == 0)
			shoot();

	if (GetAsyncKeyState(VK_LSHIFT))
		GameEngine::i()->increaseGameSpeed();

	if (GetAsyncKeyState(VK_LCONTROL))
		GameEngine::i()->decreaseGameSpeed();

}

void Player::checkCollisionWithEnemy() 
{
	for (unsigned char i = 0; i < height; ++i) {
		for (unsigned char j = 0; j < width; ++j) {
			if (model[i][j] != ' ') {
				Object* o = GameEngine::i()->getObjectAt(x + j, y + i, id);
				if (o && o->getObjectType() == ENEMY) {
					GameEngine::i()->destroyObject(o->getId());
					receiveDamage(DEFAULT_DAMAGE_COLLISION_WITH_ENEMY);
				}
			}
		}
	}
}

void Player::update()
{
	if (currFireTick > 0)
		--currFireTick;
	checkInput();
	checkCollisionWithEnemy();
}

unsigned Player::getWidth() const { return width; }

unsigned Player::getHeight() const { return height; }

unsigned char Player::receiveDamage(unsigned char damage) {
	PlaySound(TEXT("Sounds/player_hit.wav"), NULL, SND_FILENAME | SND_ASYNC);
	if (health - damage > 0) {
		health -= damage;
	} else {
		if (lives - 1 >= 0) {
			--lives;
			health = DEFAULT_HEALTH;
		} else {
			health = 0;
			GameEngine::i()->triggerGameOver();
		}
	}
	return damage;
}

void Player::receivePoints(unsigned int points) { this->score += points; }

void Player::shoot() 
{
	PlaySound(TEXT("Sounds/player_shoot.wav"), NULL, SND_FILENAME | SND_ASYNC);
	GameEngine::i()->instanciateObject(new Bullet(x + width, y + height / 2, 1, id));
	currFireTick = DEFAULT_FIRE_RATE;
}

void Player::serialize(std::ofstream & out) const 
{
	Object::serialize(out);
	out.write((const char*)&score, sizeof(score));
	out.write((const char*)&lives, sizeof(lives));
	out.write((const char*)&health, sizeof(health));
	out.write((const char*)&currFireTick, sizeof(currFireTick));
}
