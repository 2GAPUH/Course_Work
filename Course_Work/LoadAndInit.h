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
void InitHero(mainHero* Laplas);
void InitEnemys(mainEnemys levelEnemys[], int* enemysCount, mainRenderer* texture_beaver_run, mainRenderer* texture_beaver_atack,
	mainRenderer* texture_beaver_preAtack, mainRenderer* texture_krab_run, mainRenderer* texture_acid_effect,
	mainRenderer* texture_tower, mainRenderer* texture_tower_bullet, mainRenderer* texture_digit_idle, mainRenderer* texture_digit_atack,
	mainRenderer* texture_bat_with_anvil, mainRenderer* texture_bat_without_anvil, mainRenderer* texture_anvil_without_bat,
	mainRenderer* texture_barrel, mainRenderer* texture_box, mainRenderer* texture_box_bullet);
void InitTraps(mainTraps levelTraps[], int* trapsCount, mainRenderer* texture_dart_trap, mainRenderer* texture_pressure_plate,
	mainRenderer* texture_trap_spikes);
void InitItems(mainItems levelItems[], int* itemsCount, mainRenderer* texture_buff_DMG, mainRenderer* texture_item_Rubber_Bullet,
	mainRenderer* texture_barrel, mainRenderer* texture_item_Ball, mainRenderer* texture_item_acid, mainRenderer* texture_buff_speed, 
	mainRenderer* texture_buff_lucky, mainRenderer* texture_skill_figure, mainRenderer* texture_kebab, mainRenderer* texture_shop);
void loadLaplas(mainHero* Laplas, const char path[], Settings* settings);
void saveLaplas(mainHero* Laplas, const char path[], Settings* settings);
void dopInitHero(mainHero* Laplas);