#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include "common_parameters.h"
#include <time.h>

void MainMenu(GameState* gameState, mainWindow* window, SDL_Renderer* ren, SDL_Window* win);
void PauseMenu(GameState* gameState, mainWindow* window, SDL_Renderer* ren, SDL_Window* win);
void SettingsMenu(GameState* gameState, Settings* settings, mainWindow* window, SDL_Renderer* ren, SDL_Window* win);
void CreditsMenu(GameState* gameState, mainWindow* window, SDL_Renderer* ren, SDL_Window* win);
void SkillLeveling(mainHero* Laplas,  mainWindow* window, SDL_Renderer* ren, SDL_Window* win, mainTextureSkill* texture_skill);