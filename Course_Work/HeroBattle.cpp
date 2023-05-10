#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include "func.h"
#include "common_parameters.h"
#include <math.h>
#include <stdio.h>

bool CheckAtackHitbox(SDL_Rect* hero, SDL_Rect* enemy)
{
	// Вычисляем расстояние между центрами окружностей
	float distance = sqrt((hero->x - enemy->x) * (hero->x - enemy->x) + (hero->y - enemy->y) * (hero->y - enemy->y));

	// Если расстояние меньше или равно сумме радиусов, то окружности пересекаются
	if (distance <= (hero->w + hero->h) / 4 + (enemy->w + enemy->h) / 4)
		return 1;
	else
		return 0;
}



void AddNewBullet(mainHero* Laplas)
{
	for (int i = 0; i < 10; i++)
	{
		if (!Laplas->battle.shoot[i].alive)
		{
			if (Laplas->physic.gazeDirection > 0)
			{
				Laplas->battle.shoot[i].alive = 1;
				Laplas->battle.shoot[i].bulletSpeed = HERO_BULLET_SPEED;
				Laplas->battle.shoot[i].shootAtackCentere.x = Laplas->hitbox.x + Laplas->hitbox.w / 2;
				Laplas->battle.shoot[i].shootAtackCentere.y = Laplas->hitbox.y;
			}
			else if (Laplas->physic.gazeDirection < 0)
			{
				Laplas->battle.shoot[i].alive = 1;
				Laplas->battle.shoot[i].bulletSpeed = -HERO_BULLET_SPEED;
				Laplas->battle.shoot[i].shootAtackCentere.x = Laplas->hitbox.x - Laplas->hitbox.w / 2;
				Laplas->battle.shoot[i].shootAtackCentere.y = Laplas->hitbox.y;
			}

			break;
		}
	}
}

bool CheckShootHitbox(SDL_Point* shoot, SDL_Rect* enemy)
{
	// Вычисляем расстояние между центрами окружностей
	float distance = sqrt((shoot->x - enemy->x) * (shoot->x - enemy->x) + (shoot->y - enemy->y) * (shoot->y - enemy->y));

	// Если расстояние меньше или равно сумме радиусов, то окружности пересекаются
	if (distance <= (HERO_BULLET_WIDTH + HERO_BULLET_HIGHT) / 4 + (enemy->w + enemy->h) / 4)
		return 1;
	else
		return 0;
}

void HeroCommonAtack(mainHero* Laplas, int* deltaTime, int* enemysCount, mainEnemys levelEnemys[])
{
	if (Laplas->battle.commonAtack && Laplas->animationType != 3 &&Laplas->effect.timeAtackCD + Laplas->effect.atackCD > *deltaTime)
	{
		Laplas->animationType = 2;
	}
	else if(Laplas->animationType != 3)
	{
		Laplas->battle.commonAtack = 0;
		for (int i = 0; i < *enemysCount; i++)
			levelEnemys[i].effect.underAtack = 0;
		Laplas->animationType = 0;
	}

	for (int i = 0; i < *enemysCount; i++)
	{
		if (Laplas->battle.commonAtack && !levelEnemys[i].effect.underAtack && CheckAtackHitbox(&Laplas->hitbox, &levelEnemys[i].hitbox))
		{
			levelEnemys[i].effect.underAtack = 1;
			levelEnemys[i].status.HP -= Laplas->status.DMG;
			if (levelEnemys[i].status.HP <= 0)
			{
				levelEnemys[i].status.alive = 0;
			}
		}
	}
}

void HeroShootAtack(mainHero* Laplas, int* deltaTime, int* enemysCount, mainEnemys levelEnemys[])
{
	if (Laplas->battle.shootAtack && Laplas->animationType != 2 && Laplas->effect.timeShootCD + Laplas->effect.shootCD > *deltaTime)
	{
		Laplas->animationType = 3;
	}
	else if(Laplas->animationType != 2)
	{
		Laplas->battle.shootAtack = 0;
		for (int i = 0; i < *enemysCount; i++)
			levelEnemys[i].effect.underAtack = 0;
		Laplas->animationType = 0;
	}

	for (int j = 0; j < 10; j++)
		if (Laplas->battle.shoot[j].alive)
		{
			Laplas->battle.shoot[j].shootAtackCentere.x += Laplas->battle.shoot[j].bulletSpeed;
			for (int i = 0; i < *enemysCount; i++)
			{
				if (!levelEnemys[i].effect.underAtack && CheckShootHitbox(&Laplas->battle.shoot[j].shootAtackCentere, &levelEnemys[i].hitbox))
				{
					Laplas->battle.shoot[j].alive = 0;
					levelEnemys[i].effect.underAtack = 1;
					levelEnemys[i].status.HP -= Laplas->status.Shoot_DMG;
					if (levelEnemys[i].status.HP <= 0)
					{
						levelEnemys[i].status.alive = 0;
					}
				}
			}
		}
}
