#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>
#include "func.h"

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280
#define FPS 60
#define HERO_SPEED 5
#define HERO_WIDHT 20
#define HERO_HEIGHT 20
#define GRAVITY 5


SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
SDL_Surface* win_surface = NULL;
SDL_Event ev;

int main(int argc, char* argv[])
{
	int xMove = 0, yMove = 0;
	int temp = 0;
	SDL_Rect hero = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, HERO_WIDHT, HERO_HEIGHT };

	Init(&win, &ren, &win_surface, WINDOW_HEIGHT, WINDOW_WIDTH);

	

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

			case SDL_KEYDOWN:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					isRunning = false;
					break;
				case SDL_SCANCODE_A:
					xMove = -1;
					break;
				case SDL_SCANCODE_W:
					yMove = -1;
					break;
				case SDL_SCANCODE_S:
					yMove = 1;
					break;
				case SDL_SCANCODE_D:
					xMove = 1;
					break;
				}
				break;

			case SDL_KEYUP:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_A:
					xMove = 0;
					break;
				case SDL_SCANCODE_W:
					yMove = 0;
					break;
				case SDL_SCANCODE_S:
					yMove = 0;
					break;
				case SDL_SCANCODE_D:
					xMove = 0;
					break;
				}
				break;
			}
					
		}

		if (hero.x + (temp = xMove * HERO_SPEED) >= 0 && xMove == -1)
			hero.x += temp;

		if (hero.x + (temp = xMove * HERO_SPEED) + HERO_WIDHT <= WINDOW_WIDTH && xMove == 1)
			hero.x += temp;

		if (hero.y + (temp = yMove * HERO_SPEED) >= 0 && yMove == -1)
			hero.y += temp;

		if (hero.y + (temp = yMove * HERO_SPEED) + HERO_HEIGHT <= WINDOW_HEIGHT && yMove == 1)
			hero.y += temp;

		hero.y += GRAVITY;

		SDL_SetRenderDrawColor(ren, 255, 0, 128, 255);
		SDL_RenderFillRect(ren, &hero);

		SDL_RenderPresent(ren);
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		SDL_Delay(1000/FPS);
	}

	//SDL_Delay(10000);

	DeInit(0, &win, &ren, &win_surface);
	
	return 0;
}


//SDL_Surface* surface = IMG_Load("Hentai.jpg");
//
//SDL_Rect destrect = { 0, 0, 100, 100 };
//
//SDL_BlitSurface(surface, NULL, win_surface, &destrect);
//
//SDL_UpdateWindowSurface(win);