#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include "func.h"
#include "common_parameters.h"
#include <math.h>


int Det(int a, int b, int c, int d)
{
	return a * d - b * c;
}

bool Between(int a, int b, double c)
{
	return min(a, b) <= c + EPS && c <= max(a, b) + EPS;
}

bool Intersect(int a, int b, int c, int d)
{
	if (a > b)  swap(&a, &b);
	if (c > d)  swap(&c, &d);
	return max(a, c) <= min(b, d);
}

bool SegmentOverlay(SDL_Point firstStart, SDL_Point firstEnd, SDL_Point secondStart, SDL_Point secondEnd, SDL_Point* intersect)
{
	int A1 = firstStart.y - firstEnd.y, B1 = firstEnd.x - firstStart.x, C1 = -A1 * firstStart.x - B1 * firstStart.y;
	int A2 = secondStart.y - secondEnd.y, B2 = secondEnd.x - secondStart.x, C2 = -A2 * secondStart.x - B2 * secondStart.y;
	int zn = Det(A1, B1, A2, B2);
	if (zn != 0) {
		double x = -Det(C1, B1, C2, B2) * 1. / zn;
		double y = -Det(A1, C1, A2, C2) * 1. / zn;
		if (Between(firstStart.x, firstEnd.x, x) && Between(firstStart.y, firstEnd.y, y)
			&& Between(secondStart.x, secondEnd.x, x) && Between(secondStart.y, secondEnd.y, y))
		{
			intersect->x = x;
			intersect->y = y;
			return 1;
		}
		else
			return 0;
	}
	else
		return Det(A1, C1, A2, C2) == 0 && Det(B1, C1, B2, C2) == 0
		&& Intersect(firstStart.x, firstEnd.x, secondStart.x, secondEnd.x)
		&& Intersect(firstStart.y, firstEnd.y, secondStart.y, secondEnd.y);
}


bool CheckBorders(mainHero* Laplas, SDL_Rect unit)
{
	static SDL_Point intersect;
	//Верхнаяя прямая
	if (SegmentOverlay({ Laplas->hitbox.x ,Laplas->hitbox.y + Laplas->hitbox.h / 2 }, { Laplas->position.x, Laplas->position.y + Laplas->hitbox.h / 2 },
		{ unit.x - Laplas->hitbox.w / 2, unit.y }, { unit.x + unit.w + Laplas->hitbox.w / 2, unit.y }, &intersect))
	{
		Laplas->hitbox.x = Laplas->position.x;
		Laplas->hitbox.y = intersect.y - Laplas->hitbox.h / 2 - 1;
		Laplas->physic.onBorder = 1;
		//Laplas->physic.accelerationY = 0.1;
		return 0;
	}

	//Правая прямая
	if (SegmentOverlay({ Laplas->hitbox.x - Laplas->hitbox.w / 2 ,Laplas->hitbox.y }, { Laplas->position.x - Laplas->hitbox.w / 2, Laplas->position.y },
		{ unit.x + unit.w, unit.y - Laplas->hitbox.h / 2 }, { unit.x + unit.w , unit.y + unit.h + Laplas->hitbox.h / 2 }, &intersect))
	{
		Laplas->hitbox.x = intersect.x + Laplas->hitbox.w / 2 + 1;
		Laplas->hitbox.y = Laplas->position.y;
		return 0;
	}

	//Левая прямая
	if (SegmentOverlay({ Laplas->hitbox.x + Laplas->hitbox.w / 2 ,Laplas->hitbox.y }, { Laplas->position.x + Laplas->hitbox.w / 2, Laplas->position.y },
		{ unit.x, unit.y - Laplas->hitbox.h / 2 }, { unit.x , unit.y + unit.h + Laplas->hitbox.h / 2 }, &intersect))
	{
		Laplas->hitbox.x = intersect.x - Laplas->hitbox.w / 2 - 1;
		Laplas->hitbox.y = Laplas->position.y;
		return 0;
	}

	//Нижняя прямая
	if (SegmentOverlay({ Laplas->hitbox.x ,Laplas->hitbox.y - Laplas->hitbox.h / 2 }, { Laplas->position.x, Laplas->position.y - Laplas->hitbox.h / 2 },
		{ unit.x - Laplas->hitbox.w / 2, unit.y + unit.h }, { unit.x + unit.w + Laplas->hitbox.w / 2, unit.y + unit.h }, &intersect))
	{
		Laplas->hitbox.x = Laplas->position.x;
		Laplas->hitbox.y = intersect.y + Laplas->hitbox.h / 2 + 1;
		Laplas->physic.impulse = 0.1;
		Laplas->physic.accelerationY = 0.3;
		return 0;
	}

	return 1;
}

//Получение координат
void PhysicGetBase(mainHero* Laplas)
{
	Laplas->position.x = Laplas->hitbox.x;
	Laplas->position.y = Laplas->hitbox.y;
	Laplas->physic.gazeDirection = Laplas->physic.xMoveL + Laplas->physic.xMoveR;
}

//Движение по оси X + рывок
void PhysicXmovement(mainHero* Laplas)
{
	Laplas->position.x += (Laplas->physic.xMoveL + Laplas->physic.xMoveR) * Laplas->physic.speed * Laplas->physic.accelerationX;
	if (Laplas->physic.accelerationX > 5)
		Laplas->physic.accelerationX--;
	else
		Laplas->physic.accelerationX = 1;
}

//Прыжок
void PhysicJump(mainHero* Laplas)
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
void PhysicGravity(mainHero* Laplas)
{
	Laplas->position.y += Laplas->physic.gravity * Laplas->physic.accelerationY;
	if (Laplas->physic.accelerationY < 1)
		Laplas->physic.accelerationY += 0.05;
}

//Проверка на наложение хитбоксов
void PhysicHitboxOverlay(int bordersCount, mainHero* Laplas, mainBorders levelBorders[])
{
	int check = 1;
	for (int i = 0;i < bordersCount;i++)
	{
		if (levelBorders[i].type == 1 || levelBorders[i].type == 2)
			if(!CheckBorders(Laplas, levelBorders[i].bordersHitbox))
			{
			check = 0;
			break;
			}
	}

	if (check == 1)
	{
		Laplas->hitbox.x = Laplas->position.x;
		Laplas->hitbox.y = Laplas->position.y;
		Laplas->physic.onBorder = 0;
	}
}

//Выход за границы мира
void PhysicOutworldCheck(mainHero* Laplas, mainBorders levelBorders[])
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
