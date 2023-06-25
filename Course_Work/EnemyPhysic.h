#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include "func.h"
#include "common_parameters.h"
#include <math.h>


bool EnemyCheckBorders(mainEnemys* Enemy, SDL_Rect unit);
void EnemyPhysicGetBase(mainEnemys Enemy[], int* enemysCount);
void EnemyPhysicXmovement(mainEnemys* Enemy);
void EnemyPhysicJump(mainEnemys* Enemy, int* enemysCount, mainHero * Laplas);
void EnemyPhysicGravity(mainEnemys Enemy[], int* enemysCount);
void EnemyPhysicHitboxOverlay(int* bordersCount, int* enemysCount, mainEnemys Enemy[], mainBorders levelBorders[], int deltaTime, mainHero* Laplas);
void EnemyPhysicOutworldCheck(int* enemysCount, mainEnemys Enemy[], mainBorders levelBorders[]);