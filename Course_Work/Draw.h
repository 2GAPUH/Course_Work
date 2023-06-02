#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include "common_parameters.h"
#include <time.h>

void DrawMainHero(mainHero* Laplas, mainWindow window, SDL_Renderer* ren, int levelWidth, int levelHeight);
void DrawHitbox(int bordersCount, mainBorders levelBorders[], mainHero* Laplas, mainWindow* window, mainRenderer* cobbleStone,
	mainRenderer* platform, mainRenderer* trampline, SDL_Renderer* ren, int levelWidth, int levelHeight);
void DrawFakeWalls(int bordersCount, mainBorders levelBorders[], mainHero* Laplas, mainWindow* window, mainRenderer* cobbleStone
	, SDL_Renderer* ren, int levelWidth, int levelHeight);
void DrawEnemys(int* enemysCount, mainEnemys levelEnemys[], mainHero* Laplas, mainWindow* window, mainRenderer* texture_buff_DMG
	, SDL_Renderer* ren, int levelWidth, int levelHeight);
void DrawItem(mainHero* Laplas, mainItems items[], mainWindow* window, SDL_Renderer* ren, int levelWidth, int levelHeight);
void DrawTraps(int* trapsCount, mainTraps levelTraps[], mainHero* Laplas, mainWindow* window, SDL_Renderer* ren, int levelWidth, int levelHeight);
void DrawHeroBullet(mainHero* Laplas, mainWindow* window, SDL_Renderer* ren, int levelWidth, int levelHeight);
void DrawTrapsBullet(mainHero* Laplas, mainWindow* window, int* trapsCount, mainTraps levelTraps[], mainRenderer* trap_dart
	, SDL_Renderer* ren, int levelWidth, int levelHeight);
void DrawAmmoBar(mainRenderer ammoBarTexture, mainWindow* window, SDL_Renderer* ren);
void DrawLifeBar(mainHero laplas, mainRenderer hpBarTexture, mainRenderer hpBarEdgingTexture, mainWindow* window, SDL_Renderer* ren);