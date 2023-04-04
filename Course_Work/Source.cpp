#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "func.h"

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
SDL_Surface* win_surface = NULL;
SDL_Event ev;

int main(int argc, char* argv[])
{
	SDL_Surface* surface = IMG_Load("Hentai.jpg");

	Init(&win, &ren, &win_surface, WINDOW_HEIGHT, WINDOW_WIDTH);

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

	bool isRunning = true;

	while (isRunning)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;
			}
		}
		SDL_Rect destrect = { 0, 0, 100, 100 };

		SDL_BlitSurface(surface, NULL, win_surface, &destrect);

		SDL_UpdateWindowSurface(win);

		//SDL_RenderPresent(ren);
		SDL_RenderClear(ren);

		SDL_Delay(100);
	}

	//SDL_Delay(10000);

	DeInit(0, &win, &ren, &win_surface);
	
	return 0;
}