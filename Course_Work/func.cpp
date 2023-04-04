#include <SDL.h>
#include <iostream>

void DeInit(int error, SDL_Window** win, SDL_Renderer** ren)
{
	if (ren != NULL)SDL_DestroyRenderer(*ren);
	if (win != NULL)SDL_DestroyWindow(*win);
	SDL_Quit();
	exit(error);
}

void Init(SDL_Window** win, SDL_Renderer** ren, int WINDOW_HEIGHT, int WINDOW_WIDTH)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf_s("Couldn't init SDL! Error: %s\n", SDL_GetError());
		system("pause");
		DeInit(1, win, ren);
	}

	*win = SDL_CreateWindow("SDL project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (win == NULL)
	{
		printf_s("Couldn't create window! Error: %s\n", SDL_GetError());
		system("pause");
		DeInit(1, win, ren);
	}

	*ren = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED);

	if (ren == NULL)
	{
		printf_s("Couldn't create renderer! Error: %s", SDL_GetError());
		system("pause");
		DeInit(1, win, ren);
	}
}