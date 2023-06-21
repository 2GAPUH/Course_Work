#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include "common_parameters.h"
#include <time.h>

mainBorders* LoadLevel(mainBorders* levelBorders, int* bordersCount, mainHero* Laplas, const char levelName[], int* levelWidth,
	int* levelHeight);
mainEnemys* LoadEnemys(mainEnemys* levelEnemys, int* enemysCount, const char levelName[]);
mainTraps* LoadTraps(mainTraps* levelTraps, int* trapsCount, const char levelName[]);
mainItems* LoadItems(mainItems* levelItems, int* itemsCount, const char levelName[]);
mainHero InitHero();
void InitEnemys(mainEnemys levelEnemys[], int* enemysCount, mainRenderer* texture_beaver_run, mainRenderer* texture_beaver_atack,
	mainRenderer* texture_beaver_preAtack, mainRenderer* texture_krab_run);
void InitTraps(mainTraps levelTraps[], int* trapsCount, mainRenderer* texture_dart_trap, mainRenderer* texture_pressure_plate,
	mainRenderer* texture_trap_spikes);
void InitItems(mainItems levelItems[], int* itemsCount, mainRenderer* texture_buff_DMG, mainRenderer* texture_item_Rubber_Bullet,
	mainRenderer* texture_barrel, mainRenderer* texture_item_Ball);
void dopLoadHero(mainHero* Laplas);
void loadLaplas(mainHero* Laplas, const char path[], int* deltatime);
