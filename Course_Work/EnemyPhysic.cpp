#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include "func.h"
#include "common_parameters.h"
#include <math.h>





bool EnemyCheckBorders(mainEnemys* Enemy, SDL_Rect unit)
{
	static SDL_Point intersect;
	//�������� ������
	if (SegmentOverlay({ Enemy->hitbox.x ,Enemy->hitbox.y + Enemy->hitbox.h / 2 }, { Enemy->position.x, Enemy->position.y + Enemy->hitbox.h / 2 },
		{ unit.x - Enemy->hitbox.w / 2, unit.y }, { unit.x + unit.w + Enemy->hitbox.w / 2, unit.y }, &intersect))
	{
		Enemy->hitbox.x = Enemy->position.x;
		Enemy->hitbox.y = intersect.y - Enemy->hitbox.h / 2 - 1;
		Enemy->physic.onBorder = 1;
		//Enemy->physic.accelerationY = 0.1;
		return 0;
	}

	//������ ������
	if (SegmentOverlay({ Enemy->hitbox.x - Enemy->hitbox.w / 2 ,Enemy->hitbox.y }, { Enemy->position.x - Enemy->hitbox.w / 2, Enemy->position.y },
		{ unit.x + unit.w, unit.y - Enemy->hitbox.h / 2 }, { unit.x + unit.w , unit.y + unit.h + Enemy->hitbox.h / 2 }, &intersect))
	{
		Enemy->hitbox.x = intersect.x + Enemy->hitbox.w / 2 + 1;
		Enemy->hitbox.y = Enemy->position.y;
		return 0;
	}

	//����� ������
	if (SegmentOverlay({ Enemy->hitbox.x + Enemy->hitbox.w / 2 ,Enemy->hitbox.y }, { Enemy->position.x + Enemy->hitbox.w / 2, Enemy->position.y },
		{ unit.x, unit.y - Enemy->hitbox.h / 2 }, { unit.x , unit.y + unit.h + Enemy->hitbox.h / 2 }, &intersect))
	{
		Enemy->hitbox.x = intersect.x - Enemy->hitbox.w / 2 - 1;
		Enemy->hitbox.y = Enemy->position.y;
		return 0;
	}

	//������ ������
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

//��������� ���������
void EnemyPhysicGetBase(mainEnemys Enemy[], int* enemysCount)
{
	for (int i = 0; i < *enemysCount;i++)
	{
		if (Enemy[i].status.alive)
		{
			Enemy[i].position.x = Enemy[i].hitbox.x;
			Enemy[i].position.y = Enemy[i].hitbox.y;
			Enemy[i].physic.gazeDirection = Enemy[i].physic.xMoveL + Enemy[i].physic.xMoveR;
		}
	}
}

//�������� �� ��� X + �����
void EnemyPhysicXmovement(mainEnemys* Enemy)
{
	Enemy->position.x += (Enemy->physic.xMoveL + Enemy->physic.xMoveR) * Enemy->physic.speed * Enemy->physic.accelerationX;
	if (Enemy->physic.accelerationX > 5)
		Enemy->physic.accelerationX--;
	else
		Enemy->physic.accelerationX = 1;
}

//������
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

//����������
void EnemyPhysicGravity(mainEnemys Enemy[], int* enemysCount)
{
	for (int i = 0; i < *enemysCount;i++)
	{
		if (Enemy[i].status.alive)
		{
			Enemy[i].position.y += Enemy[i].physic.gravity * Enemy[i].physic.accelerationY;
			if (Enemy[i].physic.accelerationY < 1)
				Enemy[i].physic.accelerationY += 0.05;
		}
	}
}

//�������� �� ��������� ���������
void EnemyPhysicHitboxOverlay(int* bordersCount,int* enemysCount, mainEnemys Enemy[], mainBorders levelBorders[])
{
	for (int i = 0; i < *enemysCount;i++)
	{
		if (Enemy[i].status.alive)
		{
			int check = 1;
			for (int j = 0;j < *bordersCount;j++)
			{
				if (levelBorders[j].type == 1 || levelBorders[j].type == 2)
					if (!EnemyCheckBorders(&Enemy[i], levelBorders[j].bordersHitbox))
					{
						check = 0;
						break;
					}
			}

			if (check == 1)
			{
				Enemy[i].hitbox.x = Enemy[i].position.x;
				Enemy[i].hitbox.y = Enemy[i].position.y;
				Enemy[i].physic.onBorder = 0;
			}
		}
	}
}

//����� �� ������� ����
void EnemyPhysicOutworldCheck(int* enemysCount, mainEnemys Enemy[], mainBorders levelBorders[])
{
	for (int i = 0; i < *enemysCount;i++)
	{
		if (Enemy[i].status.alive)
		{
			if (Enemy[i].hitbox.x > levelBorders[1].bordersHitbox.w - Enemy[i].hitbox.w / 2 - 2)
				Enemy[i].hitbox.x = levelBorders[1].bordersHitbox.w - Enemy[i].hitbox.w / 2 - 2;
			else if (Enemy[i].hitbox.x < levelBorders[0].bordersHitbox.x + Enemy[i].hitbox.w / 2 + 2)
				Enemy[i].hitbox.x = levelBorders[0].bordersHitbox.x + Enemy[i].hitbox.w / 2 + 2;

			if (Enemy[i].hitbox.y > levelBorders[0].bordersHitbox.h - Enemy[i].hitbox.h / 2 - 2)
				Enemy[i].hitbox.y = levelBorders[0].bordersHitbox.h - Enemy[i].hitbox.h / 2 - 2;
			else if (Enemy[i].hitbox.y < levelBorders[0].bordersHitbox.y + Enemy[i].hitbox.h / 2 + 2)
				Enemy[i].hitbox.y = levelBorders[0].bordersHitbox.y + Enemy[i].hitbox.h / 2 + 2;
		}
	}
}
