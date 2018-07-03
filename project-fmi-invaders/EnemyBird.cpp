#include "EnemyBird.h"

const Color DEFAULT_BIRD_COLOR = C_DK_YELLOW;
const char* BIRD_MODEL[] =
{
	" <  ",
	"<<<<",
	" <  "
};

EnemyBird::EnemyBird(unsigned short x, unsigned short y) :
	Enemy(x, y, ENEMY_BIRD)
{
	verticalMoveRate /= 2;
	++horizontalMoveRate;
	hp *= 0.75;
	width = 4;
	height = 3;
	model = BIRD_MODEL;
	color = DEFAULT_BIRD_COLOR;
}

EnemyBird::EnemyBird(std::ifstream& in) :
	Enemy(in, ENEMY_BIRD)
{
	model = BIRD_MODEL;
	color = DEFAULT_BIRD_COLOR;
}

EnemyBird::~EnemyBird() {}
