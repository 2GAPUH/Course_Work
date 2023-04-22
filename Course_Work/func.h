#pragma once
#include <SDL.h>



void DeInit(int error, SDL_Window** win, SDL_Renderer** ren, SDL_Surface** win_surface);
void Init(SDL_Window** win, SDL_Renderer** ren, SDL_Surface** win_surface);
int min(int a, int b);
int max(int a, int b);
void swap(int* a, int* b);
void FPSControl();
bool SegmentOverlay(SDL_Point firstStart, SDL_Point firstEnd, SDL_Point secondStart, SDL_Point secondEnd, SDL_Point* intersect);
bool Intersect(int a, int b, int c, int d);
bool Between(int a, int b, double c);
int Det(int a, int b, int c, int d);