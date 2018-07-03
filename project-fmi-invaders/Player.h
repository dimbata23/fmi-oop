#ifndef __PLAYER_HEADER_INCLUDED__
#define __PLAYER_HEADER_INCLUDED__

#include "WindowHandler.h"
#include "Object.h"

// TODO: Add cooldown to the rapid shooting

class Player : public Object {
public:
	Player();
	Player(std::ifstream&);
	virtual ~Player();
public:
	unsigned int getScore() const;
	unsigned char getLives() const;
	unsigned char getHealth() const;
	virtual void update();
	virtual unsigned getWidth() const;
	virtual unsigned getHeight() const;
	virtual unsigned char receiveDamage(unsigned char damage);
	void receivePoints(unsigned int points);
	void shoot();
	virtual void serialize(std::ofstream& out) const;
protected:
	void checkInput();
	void checkCollisionWithEnemy();
protected:
	unsigned int score;
	char lives;
	char health;
	unsigned currFireTick;
};

#endif // !__PLAYER_HEADER_INCLUDED__
