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

void HeroCommonAtack(mainHero* Laplas, int* deltaTime, int* enemysCount, mainEnemys levelEnemys[])
{
	if (Laplas->battle.commonAtack && Laplas->effect.timeAtackCD + Laplas->effect.atackCD > *deltaTime)
	{
		Laplas->animationType = 2;
	}
	else
	{
		Laplas->battle.commonAtack = 0;
		for (int i = 0;i < *enemysCount; i++)
			levelEnemys[i].effect.underAtack = 0;
		Laplas->animationType = 0;
	}

	for (int i = 0; i < *enemysCount; i++)
	{
		if (Laplas->battle.commonAtack && levelEnemys[i].status.alive && !levelEnemys[i].effect.underAtack && CheckAtackHitbox(&Laplas->hitbox, &levelEnemys[i].hitbox))
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