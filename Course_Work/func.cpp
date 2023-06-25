#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <time.h>
#include "common_parameters.h"

void DeInit(int error, SDL_Window** win, SDL_Renderer** ren, SDL_Surface** win_surface)
{
	if (win_surface != NULL)SDL_FreeSurface(*win_surface);
	if (ren != NULL)SDL_DestroyRenderer(*ren);
	if (win != NULL)SDL_DestroyWindow(*win);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	exit(0);
}

void Init(SDL_Window** win, SDL_Renderer** ren, SDL_Surface** win_surface)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf_s("Couldn't init SDL! Error: %s\n", SDL_GetError());
		system("pause");
		DeInit(1, win, ren, win_surface);
	}

	int res;
	if ((res = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) == 0)
	{
		printf_s("Couldn't init SDL_Image! Error: %s\n", SDL_GetError());
		system("pause");
		DeInit(1, win, ren, win_surface);
	}
	if (res & IMG_INIT_JPG) printf_s("Init JPG library.\n"); else printf_s("Can't Init JPG library!\n");
	if (res & IMG_INIT_PNG) printf_s("Init PNG library.\n"); else printf_s("Can't Init PNG library!\n");

	if (TTF_Init())
	{
		printf_s("Couldn't init SDL_ttf! Error: %s\n", SDL_GetError());
		system("pause");
		DeInit(1, win, ren, win_surface);
	}

	*win = SDL_CreateWindow("SDL project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (win == NULL)
	{
		printf_s("Couldn't create window! Error: %s\n", SDL_GetError());
		system("pause");
		DeInit(1, win, ren, win_surface);
	}

	*ren = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED);

	if (ren == NULL)
	{
		printf_s("Couldn't create renderer! Error: %s", SDL_GetError());
		system("pause");
		DeInit(1, win, ren, win_surface);
	}

	*win_surface = SDL_GetWindowSurface(*win);
	if (win_surface == NULL)
	{
		printf_s("Couldn't create surface! Error: %s", SDL_GetError());
		system("pause");
		DeInit(1, win, ren, win_surface);
	}
}


void swap(int* a, int* b)
{
	*a ^= *b ^= *a ^= *b;
}

int max(int a, int b)
{
	if (a > b)
		return a;
	return b;
}

int min(int a, int b)
{
	if (a < b)
		return a;
	return b;
}


void FPSControl()
{
	static int time = 0;
	while (true)
	{
		if (clock() - time >= 1000 / FPS)
		{
			time = clock();
			break;
		}
		else
			SDL_Delay(1);
	}
}

int Det(int a, int b, int c, int d)
{
	return a * d - b * c;
}

bool Between(int a, int b, double c)
{
	return min(a, b) <= c + EPS && c <= max(a, b) + EPS;
}

bool Intersect(int a, int b, int c, int d)
{
	if (a > b)  swap(&a, &b);
	if (c > d)  swap(&c, &d);
	return max(a, c) <= min(b, d);
}

bool SegmentOverlay(SDL_Point firstStart, SDL_Point firstEnd, SDL_Point secondStart, SDL_Point secondEnd, SDL_Point* intersect)
{
	int A1 = firstStart.y - firstEnd.y, B1 = firstEnd.x - firstStart.x, C1 = -A1 * firstStart.x - B1 * firstStart.y;
	int A2 = secondStart.y - secondEnd.y, B2 = secondEnd.x - secondStart.x, C2 = -A2 * secondStart.x - B2 * secondStart.y;
	int zn = Det(A1, B1, A2, B2);
	if (zn != 0) {
		double x = -Det(C1, B1, C2, B2) * 1. / zn;
		double y = -Det(A1, C1, A2, C2) * 1. / zn;
		if (Between(firstStart.x, firstEnd.x, x) && Between(firstStart.y, firstEnd.y, y)
			&& Between(secondStart.x, secondEnd.x, x) && Between(secondStart.y, secondEnd.y, y))
		{
			intersect->x = x;
			intersect->y = y;
			return 1;
		}
		else
			return 0;
	}
	else
		return Det(A1, C1, A2, C2) == 0 && Det(B1, C1, B2, C2) == 0
		&& Intersect(firstStart.x, firstEnd.x, secondStart.x, secondEnd.x)
		&& Intersect(firstStart.y, firstEnd.y, secondStart.y, secondEnd.y);
}

