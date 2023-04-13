#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include "func.h"
#include "common_parameters.h"
#include <math.h>





bool CheckBorders(mainEnemys* Enemy, SDL_Rect unit)
{
	static SDL_Point intersect;
	//Верхнаяя прямая
	if (SegmentOverlay({ Enemy->hitbox.x ,Enemy->hitbox.y + Enemy->hitbox.h / 2 }, { Enemy->position.x, Enemy->position.y + Enemy->hitbox.h / 2 },
		{ unit.x - Enemy->hitbox.w / 2, unit.y }, { unit.x + unit.w + Enemy->hitbox.w / 2, unit.y }, &intersect))
	{
		Enemy->hitbox.x = Enemy->position.x;
		Enemy->hitbox.y = intersect.y - Enemy->hitbox.h / 2 - 1;
		Enemy->physic.onBorder = 1;
		//Enemy->physic.accelerationY = 0.1;
		return 0;
	}

	//Правая прямая
	if (SegmentOverlay({ Enemy->hitbox.x - Enemy->hitbox.w / 2 ,Enemy->hitbox.y }, { Enemy->position.x - Enemy->hitbox.w / 2, Enemy->position.y },
		{ unit.x + unit.w, unit.y - Enemy->hitbox.h / 2 }, { unit.x + unit.w , unit.y + unit.h + Enemy->hitbox.h / 2 }, &intersect))
	{
		Enemy->hitbox.x = intersect.x + Enemy->hitbox.w / 2 + 1;
		Enemy->hitbox.y = Enemy->position.y;
		return 0;
	}

	//Левая прямая
	if (SegmentOverlay({ Enemy->hitbox.x + Enemy->hitbox.w / 2 ,Enemy->hitbox.y }, { Enemy->position.x + Enemy->hitbox.w / 2, Enemy->position.y },
		{ unit.x, unit.y - Enemy->hitbox.h / 2 }, { unit.x , unit.y + unit.h + Enemy->hitbox.h / 2 }, &intersect))
	{
		Enemy->hitbox.x = intersect.x - Enemy->hitbox.w / 2 - 1;
		Enemy->hitbox.y = Enemy->position.y;
		return 0;
	}

	//Нижняя прямая
	if (SegmentOverlay({ Enemy->hitbox.x ,Enemy->hitbox.y - Enemy->hitbox.h / 2 }, { Enemy->position.x, Enemy->position.y - Enemy->hitbox.h / 2 },
		{ unit.x - Enemy->hitbox.w / 2, unit.y + unit.h }, { unit.x + unit.w + Enemy->hitbox.w / 2, unit.y + unit.h }, &intersect))
	{
		Enemy->hitbox.x = Enemy->position.x;
		Enemy->hitbox.y = intersect.y + Enemy->hitbox.h / 2 + 1;
		Enemy->physic.impulse = 0.1;
		Enemy->physic.accelerationY = 0.3;
		return 0;
	}

	return 1;
}

//Получение координат
void EnemyPhysicGetBase(mainEnemys* Enemy)
{
	Enemy->position.x = Enemy->hitbox.x;
	Enemy->position.y = Enemy->hitbox.y;
	Enemy->physic.gazeDirection = Enemy->physic.xMoveL + Enemy->physic.xMoveR;
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
void EnemyPhysicJump(mainEnemys* Enemy)
{
	if (Enemy->physic.impulse > 0.1)
	{
		Enemy->position.y -= 30 * Enemy->physic.impulse;
		if (Enemy->physic.impulse > 0.1)
			Enemy->physic.impulse -= 0.02;
		else
			Enemy->physic.accelerationY = 0.1;
		Enemy->physic.onBorder = 0;
	}
}

//Гравитация
void EnemyPhysicGravity(mainEnemys* Enemy)
{
	Enemy->position.y += Enemy->physic.gravity * Enemy->physic.accelerationY;
	if (Enemy->physic.accelerationY < 1)
		Enemy->physic.accelerationY += 0.05;
}

//Проверка на наложение хитбоксов
void EnemyPhysicHitboxOverlay(int bordersCount, mainEnemys* Enemy, mainBorders levelBorders[])
{
	int check = 1;
	for (int i = 0;i < bordersCount;i++)
	{
		if (levelBorders[i].type == 1 || levelBorders[i].type == 2)
			if(!CheckBorders(Enemy, levelBorders[i].bordersHitbox))
			{
			check = 0;
			break;
			}
	}

	if (check == 1)
	{
		Enemy->hitbox.x = Enemy->position.x;
		Enemy->hitbox.y = Enemy->position.y;
		Enemy->physic.onBorder = 0;
	}
}

//Выход за границы мира
void EnemyPhysicOutworldCheck(mainEnemys* Enemy, mainBorders levelBorders[])
{
	if (Enemy->hitbox.x > levelBorders[1].bordersHitbox.w - Enemy->hitbox.w / 2 - 2)
		Enemy->hitbox.x = levelBorders[1].bordersHitbox.w - Enemy->hitbox.w / 2 - 2;
	else if (Enemy->hitbox.x < levelBorders[0].bordersHitbox.x + Enemy->hitbox.w / 2 + 2)
		Enemy->hitbox.x = levelBorders[0].bordersHitbox.x + Enemy->hitbox.w / 2 + 2;

	if (Enemy->hitbox.y > levelBorders[0].bordersHitbox.h - Enemy->hitbox.h / 2 - 2)
		Enemy->hitbox.y = levelBorders[0].bordersHitbox.h - Enemy->hitbox.h / 2 - 2;
	else if (Enemy->hitbox.y < levelBorders[0].bordersHitbox.y + Enemy->hitbox.h / 2 + 2)
		Enemy->hitbox.y = levelBorders[0].bordersHitbox.y + Enemy->hitbox.h / 2 + 2;
}
