#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include "common_parameters.h"
#include <time.h>

void DrawMainHero(mainHero* Laplas, mainWindow window, SDL_Renderer* ren, int levelWidth, int levelHeight);
void DrawHitbox(int bordersCount, mainBorders levelBorders[], mainHero* Laplas, mainWindow* window, mainRenderer* cobbleStone,
	mainRenderer* platform, mainRenderer* trampline, mainRenderer* tmp_Platform, SDL_Renderer* ren, int levelWidth, int levelHeight);
void DrawFakeWalls(int bordersCount, mainBorders levelBorders[], mainHero* Laplas, mainWindow* window, mainRenderer* cobbleStone
	, SDL_Renderer* ren, int levelWidth, int levelHeight);
void DrawEnemys(int* enemysCount, mainEnemys levelEnemys[], mainHero* Laplas, mainWindow* window, mainRenderer* texture_buff_DMG
	, SDL_Renderer* ren, int levelWidth, int levelHeight);
void DrawItem(mainHero* Laplas, mainItems items[], mainWindow* window, SDL_Renderer* ren, int levelWidth, int levelHeight, 
	int* itemsCount, TTF_Font* font);
void DrawTraps(int* trapsCount, mainTraps levelTraps[], mainHero* Laplas, mainWindow* window, SDL_Renderer* ren, int levelWidth, int levelHeight);
void DrawHeroBullet(mainHero* Laplas, mainWindow* window, SDL_Renderer* ren, int levelWidth, int levelHeight);
void DrawTrapsBullet(mainHero* Laplas, mainWindow* window, int* trapsCount, mainTraps levelTraps[], mainRenderer* trap_dart
	, SDL_Renderer* ren, int levelWidth, int levelHeight);
void DrawAmmoBar(mainRenderer ammoBarTexture, mainWindow* window, SDL_Renderer* ren);
void DrawLifeBar(mainHero laplas, mainRenderer hpBarTexture, mainRenderer hpBarEdgingTexture, mainWindow* window, SDL_Renderer* ren);
void DrawBuffsEffect(SDL_Renderer* ren, mainHero* Laplas, int levelWidth, int levelHeight, mainWindow* window);
void DrawItemsEffect(SDL_Renderer* ren, mainHero* Laplas, int levelWidth, int levelHeight, mainWindow* window);
void DrawEnemyHP(mainHero Laplas, mainEnemys* levelEnemys, int enemysCount, mainRenderer hpBarTexture, mainRenderer enemyHpBarEdgingTexture, mainWindow* window, SDL_Renderer* ren);
void DrawMoney(SDL_Renderer* ren, mainWindow* window, TTF_Font* font, mainHero Laplas);