#pragma once
#include <SDL.h>

void DeInit(int error, SDL_Window** win, SDL_Renderer** ren);
void Init(SDL_Window** win, SDL_Renderer** ren, int WINDOW_HEIGHT = 600, int WINDOW_WIDTH = 800);