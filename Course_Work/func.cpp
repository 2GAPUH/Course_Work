#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>

void DeInit(int error, SDL_Window** win, SDL_Renderer** ren, SDL_Surface** win_surface)
{
	if (win_surface != NULL)SDL_FreeSurface(*win_surface);
	if (ren != NULL)SDL_DestroyRenderer(*ren);
	if (win != NULL)SDL_DestroyWindow(*win);
	IMG_Quit();
	SDL_Quit();
	exit(error);
}

void Init(SDL_Window** win, SDL_Renderer** ren, SDL_Surface** win_surface, int WINDOW_HEIGHT, int WINDOW_WIDTH)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf_s("Couldn't init SDL! Error: %s\n", SDL_GetError());
		system("pause");
		DeInit(1, win, ren, win_surface);
	}

	int res;
	if (res = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0)
	{
		printf_s("Couldn't init SDL_Image! Error: %s\n", SDL_GetError());
		system("pause");
		DeInit(1, win, ren, win_surface);
	}
	if (res & IMG_INIT_JPG) printf_s("Init JPG library.\n"); else printf_s("Can't Init JPG library!\n");
	if (res & IMG_INIT_PNG) printf_s("Init PNG library.\n"); else printf_s("Can't Init PNG library!\n");


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