#include <SDL.h>
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



int main(int argc, char* argv[])
{
	Init(&win, &ren, WINDOW_HEIGHT, WINDOW_WIDTH);

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

	while (true)
	{

		SDL_RenderPresent(ren);
		SDL_RenderClear(ren);
	}

	//SDL_Delay(10000);

	DeInit(0, &win, &ren);

	return 0;
}