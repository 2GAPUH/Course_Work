#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include "common_parameters.h"
#include <time.h>
#include "func.h"
#include <string.h>

void MainMenu(GameState* gameState, mainWindow* window, SDL_Renderer* ren, SDL_Window* win);
void PauseMenu(GameState* gameState, mainWindow* window, SDL_Renderer* ren, SDL_Window* win);
void SettingsMenu(GameState* gameState, Settings* settings, mainWindow* window, SDL_Renderer* ren, SDL_Window* win, Audio audio);
void CreditsMenu(GameState* gameState, mainWindow* window, SDL_Renderer* ren, SDL_Window* win);
void SkillLeveling(mainHero* Laplas,  mainWindow* window, SDL_Renderer* ren, SDL_Window* win, mainTextureSkill* texture_skill,
	TTF_Font* font);
void UpSkillLvl(SDL_Point* mousePoint, SDL_Rect button, int* skill_lvl, int* souls);