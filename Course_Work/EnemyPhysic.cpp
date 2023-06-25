#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include "func.h"
#include "common_parameters.h"
#include <math.h>





bool EnemyCheckBorders(mainEnemys* Enemy, SDL_Rect unit)
{
	static SDL_Point intersect;
	//Верхнаяя прямая
	if (SegmentOverlay({ Enemy->hitbox.x ,Enemy->hitbox.y + Enemy->hitbox.h / 2 }, { Enemy->position.x, Enemy->position.y + Enemy->hitbox.h / 2 },
		{ unit.x - Enemy->hitbox.w / 2, unit.y }, { unit.x + unit.w + Enemy->hitbox.w / 2, unit.y }, &intersect))
	{
		Enemy->position.x = Enemy->position.x;
		Enemy->position.y = intersect.y - Enemy->hitbox.h / 2 - 1;
		Enemy->physic.onBorder = 1;
		//Enemy->physic.accelerationY = 0.1;
		return 0;
	}

	//Правая прямая
	if (SegmentOverlay({ Enemy->hitbox.x - Enemy->hitbox.w / 2 ,Enemy->hitbox.y }, { Enemy->position.x - Enemy->hitbox.w / 2, Enemy->position.y },
		{ unit.x + unit.w, unit.y - Enemy->hitbox.h / 2 }, { unit.x + unit.w , unit.y + unit.h + Enemy->hitbox.h / 2 }, &intersect))
	{
		Enemy->position.x = intersect.x + Enemy->hitbox.w / 2 + 1;
		Enemy->position.y = Enemy->position.y;
		return 0;
	}

	//Левая прямая
	if (SegmentOverlay({ Enemy->hitbox.x + Enemy->hitbox.w / 2 ,Enemy->hitbox.y }, { Enemy->position.x + Enemy->hitbox.w / 2, Enemy->position.y },
		{ unit.x, unit.y - Enemy->hitbox.h / 2 }, { unit.x , unit.y + unit.h + Enemy->hitbox.h / 2 }, &intersect))
	{
		Enemy->position.x = intersect.x - Enemy->hitbox.w / 2 - 1;
		Enemy->position.y = Enemy->position.y;
		return 0;
	}

	//Нижняя прямая
	if (SegmentOverlay({ Enemy->hitbox.x ,Enemy->hitbox.y - Enemy->hitbox.h / 2 }, { Enemy->position.x, Enemy->position.y - Enemy->hitbox.h / 2 },
		{ unit.x - Enemy->hitbox.w / 2, unit.y + unit.h }, { unit.x + unit.w + Enemy->hitbox.w / 2, unit.y + unit.h }, &intersect))
	{
		Enemy->position.x = Enemy->position.x;
		Enemy->position.y = intersect.y + Enemy->hitbox.h / 2 + 2;
		Enemy->physic.impulse = 0.1;
		Enemy->physic.accelerationY = 0.3;
		return 0;
	}

	return 1;
}

//Получение координат
void EnemyPhysicGetBase(mainEnemys Enemy[], int* enemysCount)
{
	for (int i = 0; i < *enemysCount;i++)
	{
		
		Enemy[i].position.x = Enemy[i].hitbox.x;
		Enemy[i].position.y = Enemy[i].hitbox.y;
		Enemy[i].physic.gazeDirection = Enemy[i].physic.xMoveL + Enemy[i].physic.xMoveR;
		
	}
}

//Движение по оси X + рывок
void EnemyPhysicXmovement(mainEnemys* Enemy)
{
	Enemy->position.x += (Enemy->physic.xMoveL + Enemy->physic.xMoveR) * Enemy->physic.speed * Enemy->physic.accelerationX;
	if (Enemy->physic.accelerationX > 5)
		Enemy->physic.accelerationX--;
	else
		Enemy->physic.accelerationX = 1;
}

//Прыжок
void EnemyPhysicJump(mainEnemys* levelEnemys, int* enemysCount, mainHero* Laplas)
{
	for(int i = 0; i < *enemysCount; i++)
		if (levelEnemys[i].physic.impulse > 0.1)
		{
			if (levelEnemys[i].hitbox.x + levelEnemys[i].physic.speed < Laplas->hitbox.x)
			{
				levelEnemys[i].position.x -= levelEnemys[i].physic.speed * 2;
			}
			else if (levelEnemys[i].hitbox.x - levelEnemys[i].physic.speed > Laplas->hitbox.x)
			{
				levelEnemys[i].position.x += levelEnemys[i].physic.speed * 2;
			}

			levelEnemys[i].position.y -= 30 * levelEnemys[i].physic.impulse;
			if (levelEnemys[i].physic.impulse > 0.1)
				levelEnemys[i].physic.impulse -= 0.02;
			else
				levelEnemys[i].physic.accelerationY = 0.1;
			levelEnemys[i].physic.onBorder = 0;
		}
}

//Гравитация
void EnemyPhysicGravity(mainEnemys Enemy[], int* enemysCount)
{
	for (int i = 0; i < *enemysCount;i++)
	{
		if (Enemy[i].type != 6)
		{
			Enemy[i].position.y += Enemy[i].physic.gravity * Enemy[i].physic.accelerationY;
			if (Enemy[i].physic.accelerationY < 1)
				Enemy[i].physic.accelerationY += 0.05;
		}
		
	}
}

//Проверка на наложение хитбоксов
void EnemyPhysicHitboxOverlay(int* bordersCount,int* enemysCount, mainEnemys Enemy[], mainBorders levelBorders[],int deltaTime, mainHero* Laplas)
{
	for (int i = 0; i < *enemysCount; i++)
	{
		if (Enemy[i].type != 6)
		{
			int check = 1;
			for (int j = 0; j < *bordersCount; j++)
			{
				if (levelBorders[j].type == 1 || levelBorders[j].type == 2 || levelBorders[j].type == 11 || levelBorders[j].type == 12 || levelBorders[j].type == 13 || levelBorders[j].type == 14)
				{
					if (!EnemyCheckBorders(&Enemy[i], levelBorders[j].bordersHitbox))
					{
						check = 0;
					}
				}

				else if (((levelBorders[j].type == 4 || levelBorders[j].type == 8) && (Laplas->position.y < Enemy[i].position.y || !Enemy[i].triggered)))
				{
					if (!EnemyCheckBorders(&Enemy[i], levelBorders[j].bordersHitbox))
					{
						check = 0;
					}
				}


				else if (levelBorders[j].type == 5)
				{
					if (!EnemyCheckBorders(&Enemy[i], levelBorders[j].bordersHitbox))
					{
						Enemy[i].physic.impulse = 1;
					}
				}

				/*else if ((levelBorders[j].type == 7 && levelBorders[i].alive))
				{
					if (!EnemyCheckBorders(&Enemy[i], levelBorders[j].bordersHitbox))
					{
						check = 0;

						if (!levelBorders[j].active)
						{
							levelBorders[j].active = 1;
							levelBorders[j].timer = deltaTime;
						}
					}
					if (deltaTime - levelBorders[j].timer > TMP_PLATFORM_LIFE_TIME && levelBorders[j].active)
						levelBorders[j].alive = 0;
				}

				else if (levelBorders[j].type == 7 && !levelBorders[i].alive && deltaTime - levelBorders[i].timer > TMP_PLATFORM_LIFE_TIME * 4)
				{
					levelBorders[j].active = 0;
					levelBorders[j].alive = 1;
				}*/

				else if ((levelBorders[i].type == 7 && Enemy[i].physic.impulse < 0.1 && levelBorders[i].alive))
				{
					if (!EnemyCheckBorders(&Enemy[i], levelBorders[i].bordersHitbox))
					{
						check = 0;

						if (!levelBorders[i].active)
						{
							levelBorders[i].active = 1;
							levelBorders[i].timer = deltaTime;
						}
					}
					if (deltaTime - levelBorders[i].timer > TMP_PLATFORM_LIFE_TIME && levelBorders[i].active)
						levelBorders[i].alive = 0;
				}
				else if (levelBorders[i].type == 7 && !levelBorders[i].alive && deltaTime - levelBorders[i].timer > TMP_PLATFORM_LIFE_TIME * 4)
				{
					levelBorders[i].active = 0;
					levelBorders[i].alive = 1;
				}
			}

			Enemy[i].hitbox.x = Enemy[i].position.x;
			Enemy[i].hitbox.y = Enemy[i].position.y;

			if (check == 1)
			{

				Enemy[i].physic.onBorder = 0;
			}

		}
	}
}

//Выход за границы мира
void EnemyPhysicOutworldCheck(int* enemysCount, mainEnemys Enemy[], mainBorders levelBorders[])
{
	for (int i = 0; i < *enemysCount;i++)
	{

		if (Enemy[i].hitbox.x > levelBorders[1].bordersHitbox.w - Enemy[i].hitbox.w / 2 - 2)
			Enemy[i].hitbox.x = levelBorders[1].bordersHitbox.w - Enemy[i].hitbox.w / 2 - 2;
		else if (Enemy[i].hitbox.x < levelBorders[0].bordersHitbox.x + Enemy[i].hitbox.w / 2 + 2)
			Enemy[i].hitbox.x = levelBorders[0].bordersHitbox.x + Enemy[i].hitbox.w / 2 + 2;

		if (Enemy[i].hitbox.y > levelBorders[0].bordersHitbox.h - Enemy[i].hitbox.h / 2 - 2)
			Enemy[i].hitbox.y = levelBorders[0].bordersHitbox.h - Enemy[i].hitbox.h / 2 - 2;
		else if (Enemy[i].hitbox.y < levelBorders[0].bordersHitbox.y + Enemy[i].hitbox.h / 2 + 2)
			Enemy[i].hitbox.y = levelBorders[0].bordersHitbox.y + Enemy[i].hitbox.h / 2 + 2;

		if (Enemy[i].type == 6)
		{
			if (Enemy[i].hitbox.x > levelBorders[1].bordersHitbox.w - Enemy[i].hitbox.w / 2 - 2)
				Enemy[i].status.alive = 0;
			else if (Enemy[i].hitbox.x < levelBorders[0].bordersHitbox.x + Enemy[i].hitbox.w / 2 + 2)
				Enemy[i].status.alive = 0;

			if (Enemy[i].hitbox.y > levelBorders[0].bordersHitbox.h - Enemy[i].hitbox.h / 2 - 2)
				Enemy[i].status.alive = 0;
			else if (Enemy[i].hitbox.y < levelBorders[0].bordersHitbox.y + Enemy[i].hitbox.h / 2 + 2)
				Enemy[i].status.alive = 0;
			
		}
	}
}
