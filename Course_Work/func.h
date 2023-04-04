#pragma once
#include <SDL.h>

void DeInit(int error, SDL_Window** win, SDL_Renderer** ren, SDL_Surface** surface);
void Init(SDL_Window** win, SDL_Renderer** ren, SDL_Surface** surface, int WINDOW_HEIGHT, int WINDOW_WIDTH);