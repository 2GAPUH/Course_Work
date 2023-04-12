#pragma once
#include <SDL.h>



void DeInit(int error, SDL_Window** win, SDL_Renderer** ren, SDL_Surface** win_surface);
void Init(SDL_Window** win, SDL_Renderer** ren, SDL_Surface** win_surface);
int min(int a, int b);
int max(int a, int b);
void swap(int* a, int* b);
void FPSControl();