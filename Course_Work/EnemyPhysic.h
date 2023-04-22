#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include "func.h"
#include "common_parameters.h"
#include <math.h>


bool EnemyCheckBorders(mainEnemys* Enemy, SDL_Rect unit);
void EnemyPhysicGetBase(mainEnemys* Enemy);
void EnemyPhysicXmovement(mainEnemys* Enemy);
void EnemyPhysicJump(mainEnemys* Enemy);
void EnemyPhysicGravity(mainEnemys* Enemy);
void EnemyPhysicHitboxOverlay(int bordersCount, mainEnemys* Enemy, mainBorders levelBorders[]);
void EnemyPhysicOutworldCheck(mainEnemys* Enemy, mainBorders levelBorders[]);