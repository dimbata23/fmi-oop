#include "Bullet.h"
#include "GameEngine.h"
#include "Explosion.h"

const short DEFAULT_SPEED = 2;
const short DEFAULT_DAMAGE = 4;
const unsigned short DEFAULT_BULLET_WIDTH = 1;
const unsigned short DEFAULT_BULLET_HEIGHT = 1;

const char* BULLET_MODEL[] = { "-" };

Bullet::Bullet(unsigned short x, unsigned short y,
	short direction, unsigned shooterId) :
	Object(x, y, DEFAULT_BULLET_WIDTH, DEFAULT_BULLET_HEIGHT, BULLET),
	direction(direction),
	shooterId(shooterId),
	speed(DEFAULT_SPEED),
	damage(DEFAULT_DAMAGE)
{
	model = BULLET_MODEL;
	if (direction == -1)
		color = C_RED;
	else
		color = C_LT_GRAY;
}

Bullet::Bullet(unsigned short x, unsigned short y,
	short direction, unsigned shooterId,
	unsigned short damage) :
	Object(x, y, DEFAULT_BULLET_WIDTH, DEFAULT_BULLET_HEIGHT, BULLET),
	direction(direction),
	shooterId(shooterId),
	speed(DEFAULT_SPEED),
	damage(damage)
{
	model = BULLET_MODEL;
	if (direction == -1)
		color = C_RED;
	else
		color = C_LT_GRAY;
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
	for (size_t i = 1; i <= speed; i++) {
		if ((x + (direction * i)) >= 0 && (x + (direction * i)) < WindowHandler::i()->getGameWindowWidth()) {
			if (WindowHandler::i()->lookAt(x + direction * i, y) != ' ') {
				Object* other = GameEngine::i()->getObjectAt(x + direction * i, y);
				if (other) {
					if ((other->getObjectType() == ENEMY && direction == 1) ||
						(other->getObjectType() == PLAYER && direction == -1))
					{
						other->receiveDamage(damage);
						GameEngine::i()->instanciateObject(new Explosion(x + width / 2 + direction * i, y + height / 2, EXPLOSION_TINY));
						GameEngine::i()->destroyObject(id);
						PlaySound(TEXT("Sounds/bullet_hit.wav"), NULL, SND_FILENAME | SND_ASYNC);
						return;
					}
				}
			}
		}
	}

	x += direction * speed;
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

//char Bullet::getDirection() const {
//	return direction;
//}
//
//unsigned char Bullet::getDamage() const {
//	return damage;
//}
