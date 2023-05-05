#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include "func.h"
#include "common_parameters.h"
#include <math.h>


void HeroPhysicOutworldCheck(mainHero* Laplas, mainBorders levelBorders[]);
void HeroPhysicHitboxOverlay(int* bordersCount, mainHero* Laplas, mainBorders levelBorders[]);
void HeroPhysicGravity(mainHero* Laplas);
void HeroPhysicJump(mainHero* Laplas);
void HeroPhysicXmovement(mainHero* Laplas);
void HeroPhysicGetBase(mainHero* Laplas);
bool HeroCheckBorders(mainHero* Laplas, SDL_Rect unit);