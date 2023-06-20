#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include "common_parameters.h"
#include <time.h>
#include "HeroBattle.h"

bool HeroPhysicInRange(SDL_Point unit, SDL_Rect bordersHitbox);
void HeroBulletHitboxInRange(mainHero* Laplas, int* bordersCount, mainBorders levelBorders[]);
void TrapBulletHitboxInRange(mainTraps levelTraps[], int* trapsCount, int* bordersCount, mainBorders levelBorders[]);
void HeroBulletOutworldCheck(mainHero* Laplas, mainBorders levelBorders[]);
void TrapBulletOutworldCheck(mainTraps levelTraps[], int* trapsCount, mainBorders levelBorders[]);
void EnemyTrigger(mainEnemys levelEnemys[], mainHero* Laplas, int* enemysCount);
void EnemysMovement(int* enemysCount, mainEnemys levelEnemys[], mainHero* Laplas);
void ItemEquip(mainHero* Laplas, mainItems items[]);
void EnemyDeath(int* enemysCount, mainEnemys levelEnemys[]);
void TrapActivate(int* trapsCount, mainTraps levelTraps[], mainHero* Laplas, int* deltaTime);
void TrapAtack(int* trapsCount, mainTraps levelTraps[], mainHero* Laplas, int* deltaTime);