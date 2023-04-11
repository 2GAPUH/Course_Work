#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include "func.h"
#include "common_parameters.h"
#include <math.h>


void PhysicOutworldCheck(mainHero* Laplas, mainBorders levelBorders[]);
void PhysicHitboxOverlay(int bordersCount, mainHero* Laplas, mainBorders levelBorders[]);
void PhysicGravity(mainHero* Laplas);
void PhysicJump(mainHero* Laplas);
void PhysicXmovement(mainHero* Laplas);
void PhysicGetBase(mainHero* Laplas);
bool CheckBorders(mainHero* Laplas, SDL_Rect unit);
bool SegmentOverlay(SDL_Point firstStart, SDL_Point firstEnd, SDL_Point secondStart, SDL_Point secondEnd, SDL_Point* intersect);
bool Intersect(int a, int b, int c, int d);
bool Between(int a, int b, double c);
int Det(int a, int b, int c, int d);