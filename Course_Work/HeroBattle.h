#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include "func.h"
#include "common_parameters.h"
#include <math.h>
#include <stdio.h>

bool CheckAtackHitbox(SDL_Rect* hero, SDL_Rect* enemy);
void HeroCommonAtack(mainHero* Laplas, int* deltaTime, int* enemysCount, mainEnemys levelEnemys[]);