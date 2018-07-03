#include "EnemyArrow.h"

const Color DEFAULT_ARROW_COLOR = C_DK_GREEN;
const char* ARROW_MODEL[] =
{
	"    /",
	"<(-)-",
	"    \\"
};

EnemyArrow::EnemyArrow(unsigned short x, unsigned short y) : 
	Enemy(x, y, ENEMY_ARROW)
{ 
	horizontalMoveRate /= 2;
	verticalMoveRate *= 10;
	directionRate /= 2;
	fireRate /= 3;
	value *= 5;
	hp = 1;
	width = 5;
	height = 3;
	model = ARROW_MODEL;
	color = DEFAULT_ARROW_COLOR;
}

EnemyArrow::EnemyArrow(std::ifstream& in) :
	Enemy(in, ENEMY_ARROW)
{
	model = ARROW_MODEL;
	color = DEFAULT_ARROW_COLOR;
}

EnemyArrow::~EnemyArrow() {}
