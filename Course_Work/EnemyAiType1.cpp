#include "common_parameters.h"

void EnemyAiType1logic(mainEnemys* enemy, mainHero hero) {
	if (enemy->hitbox.x<hero.hitbox.x)
	{
		enemy->hitbox.x += enemy->physic.speed;
	}
	else
	{
		enemy->hitbox.x -= enemy->physic.speed;
	}
}