#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include "func.h"
#include "common_parameters.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

bool HeroCheckBorders(mainHero* Laplas, SDL_Rect unit)
{
	static SDL_Point intersect;
	//Верхнаяя прямая
	if (SegmentOverlay({ Laplas->hitbox.x ,Laplas->hitbox.y + Laplas->hitbox.h / 2 }, { Laplas->position.x, Laplas->position.y + Laplas->hitbox.h / 2 },
		{ unit.x - Laplas->hitbox.w / 2, unit.y }, { unit.x + unit.w + Laplas->hitbox.w / 2, unit.y }, &intersect))
	{
		Laplas->position.x = Laplas->position.x;
		Laplas->position.y = intersect.y - Laplas->hitbox.h / 2 - 1;
		Laplas->physic.onBorder = 1;
		//Laplas->physic.accelerationY = 0.1;
		return 0;
	}

	//Правая прямая
	if (SegmentOverlay({ Laplas->hitbox.x - Laplas->hitbox.w / 2 ,Laplas->hitbox.y }, { Laplas->position.x - Laplas->hitbox.w / 2, Laplas->position.y },
		{ unit.x + unit.w, unit.y - Laplas->hitbox.h / 2 }, { unit.x + unit.w , unit.y + unit.h + Laplas->hitbox.h / 2 }, &intersect))
	{
		Laplas->position.x = intersect.x + Laplas->hitbox.w / 2 + 1;
		Laplas->position.y = Laplas->position.y;
		return 0;
	}

	//Левая прямая
	if (SegmentOverlay({ Laplas->hitbox.x + Laplas->hitbox.w / 2 ,Laplas->hitbox.y }, { Laplas->position.x + Laplas->hitbox.w / 2, Laplas->position.y },
		{ unit.x, unit.y - Laplas->hitbox.h / 2 }, { unit.x , unit.y + unit.h + Laplas->hitbox.h / 2 }, &intersect))
	{
		Laplas->position.x = intersect.x - Laplas->hitbox.w / 2 - 1;
		Laplas->position.y = Laplas->position.y;
		return 0;
	}

	//Нижняя прямая
	if (SegmentOverlay({ Laplas->hitbox.x ,Laplas->hitbox.y - Laplas->hitbox.h / 2 }, { Laplas->position.x, Laplas->position.y - Laplas->hitbox.h / 2 },
		{ unit.x - Laplas->hitbox.w / 2, unit.y + unit.h }, { unit.x + unit.w + Laplas->hitbox.w / 2, unit.y + unit.h }, &intersect))
	{
		Laplas->position.x = Laplas->position.x;
		Laplas->position.y = intersect.y + Laplas->hitbox.h / 2 + 1;
		Laplas->physic.impulse = 0.1;
		Laplas->physic.accelerationY = 0.3;
		return 0;
	}

	return 1;
}

bool HeroCheckBordersWithoutColision(mainHero* Laplas, SDL_Rect unit)
{
	static SDL_Point intersect;
	//Верхнаяя прямая
	if (SegmentOverlay({ Laplas->hitbox.x ,Laplas->hitbox.y + Laplas->hitbox.h / 2 }, { Laplas->position.x, Laplas->position.y + Laplas->hitbox.h / 2 },
		{ unit.x - Laplas->hitbox.w / 2, unit.y }, { unit.x + unit.w + Laplas->hitbox.w / 2, unit.y }, &intersect))
	{
		return 0;
	}

	//Правая прямая
	if (SegmentOverlay({ Laplas->hitbox.x - Laplas->hitbox.w / 2 ,Laplas->hitbox.y }, { Laplas->position.x - Laplas->hitbox.w / 2, Laplas->position.y },
		{ unit.x + unit.w, unit.y - Laplas->hitbox.h / 2 }, { unit.x + unit.w , unit.y + unit.h + Laplas->hitbox.h / 2 }, &intersect))
	{
		return 0;
	}

	//Левая прямая
	if (SegmentOverlay({ Laplas->hitbox.x + Laplas->hitbox.w / 2 ,Laplas->hitbox.y }, { Laplas->position.x + Laplas->hitbox.w / 2, Laplas->position.y },
		{ unit.x, unit.y - Laplas->hitbox.h / 2 }, { unit.x , unit.y + unit.h + Laplas->hitbox.h / 2 }, &intersect))
	{
		return 0;
	}

	//Нижняя прямая
	if (SegmentOverlay({ Laplas->hitbox.x ,Laplas->hitbox.y - Laplas->hitbox.h / 2 }, { Laplas->position.x, Laplas->position.y - Laplas->hitbox.h / 2 },
		{ unit.x - Laplas->hitbox.w / 2, unit.y + unit.h }, { unit.x + unit.w + Laplas->hitbox.w / 2, unit.y + unit.h }, &intersect))
	{
		return 0;
	}

	return 1;
}


//Получение координат
void HeroPhysicGetBase(mainHero* Laplas)
{
	Laplas->position.x = Laplas->hitbox.x;
	Laplas->position.y = Laplas->hitbox.y;
	if (Laplas->physic.xMoveL)
		Laplas->physic.gazeDirection = -1;
	if (Laplas->physic.xMoveR)
		Laplas->physic.gazeDirection = 1;
}

//Движение по оси X + рывок
void HeroPhysicXmovement(mainHero* Laplas)
{
	Laplas->position.x += (Laplas->physic.xMoveL + Laplas->physic.xMoveR) * Laplas->physic.speed * Laplas->physic.accelerationX;
	if (Laplas->physic.accelerationX > 5)
		Laplas->physic.accelerationX--;
	else
		Laplas->physic.accelerationX = 1;
}

//Прыжок
void HeroPhysicJump(mainHero* Laplas)
{
	if (Laplas->physic.impulse > 0.1)
	{
		Laplas->position.y -= 30 * Laplas->physic.impulse;
		if (Laplas->physic.impulse > 0.1)
			Laplas->physic.impulse -= 0.02;
		else
			Laplas->physic.accelerationY = 0.1;
		Laplas->physic.onBorder = 0;
	}

}

//Гравитация
void HeroPhysicGravity(mainHero* Laplas)
{
	Laplas->position.y += Laplas->physic.gravity * Laplas->physic.accelerationY;
	if (Laplas->physic.accelerationY < 1)
		Laplas->physic.accelerationY += 0.05;
}

//Проверка на наложение хитбоксов
void HeroPhysicHitboxOverlay(int* bordersCount, mainHero* Laplas, mainBorders levelBorders[], int* trapsCount, mainTraps levelTraps[], int deltaTime)
{
	int check = 1;
	for (int i = 0;i  < *bordersCount;i++)
	{
		if (levelBorders[i].type == 1 || levelBorders[i].type == 2)
		{
			if (!HeroCheckBorders(Laplas, levelBorders[i].bordersHitbox))
			{
				check = 0;
			}
		}
		
		else if((levelBorders[i].type == 4 && !Laplas->keys.pressed_S && Laplas->physic.impulse < 0.1 ))
		{
			if (!HeroCheckBorders(Laplas, levelBorders[i].bordersHitbox))
			{
				check = 0;
			}
		}


		else if (levelBorders[i].type == 5)
		{
			if (!HeroCheckBorders(Laplas, levelBorders[i].bordersHitbox))
			{
				Laplas->physic.impulse = 1;
			}
		}

		else if ((levelBorders[i].type == 7 && !Laplas->keys.pressed_S && Laplas->physic.impulse < 0.1 && levelBorders[i].alive))
		{
			if (!HeroCheckBorders(Laplas, levelBorders[i].bordersHitbox))
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

	for (int i = 0; i < *trapsCount; i++)
	{
		if (levelTraps[i].type == 1)
		{
			if (HeroCheckBorders(Laplas, levelTraps[i].hitbox))
			{
				check = 0;
			}
		}

		else if(levelTraps[i].type == 2)
			if (!HeroCheckBordersWithoutColision(Laplas, levelTraps[i].hitbox))
			{
				check = 0;
				levelTraps[i].triggered = 1;
			}
			else
				levelTraps[i].triggered = 0;

		else if (levelTraps[i].type == 3)
			if (!HeroCheckBordersWithoutColision(Laplas, levelTraps[i].hitbox))
			{
				check = 0;
				levelTraps[i].triggered = 1;
			}
			else
				levelTraps[i].triggered = 0;
	}

	Laplas->hitbox.x = Laplas->position.x;
	Laplas->hitbox.y = Laplas->position.y;

	if (check == 1)
	{
		Laplas->physic.onBorder = 0;
	}
}


//Выход за границы мира
void HeroPhysicOutworldCheck(mainHero* Laplas, mainBorders levelBorders[])
{
	if (Laplas->hitbox.x > levelBorders[1].bordersHitbox.w - Laplas->hitbox.w / 2 - 2)
		Laplas->hitbox.x = levelBorders[1].bordersHitbox.w - Laplas->hitbox.w / 2 - 2;
	else if (Laplas->hitbox.x < levelBorders[0].bordersHitbox.x + Laplas->hitbox.w / 2 + 2)
		Laplas->hitbox.x = levelBorders[0].bordersHitbox.x + Laplas->hitbox.w / 2 + 2;

	if (Laplas->hitbox.y > levelBorders[0].bordersHitbox.h - Laplas->hitbox.h / 2 - 2)
		Laplas->hitbox.y = levelBorders[0].bordersHitbox.h - Laplas->hitbox.h / 2 - 2;
	else if (Laplas->hitbox.y < levelBorders[0].bordersHitbox.y + Laplas->hitbox.h / 2 + 2)
		Laplas->hitbox.y = levelBorders[0].bordersHitbox.y + Laplas->hitbox.h / 2 + 2;
}
