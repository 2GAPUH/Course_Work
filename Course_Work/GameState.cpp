#include "GameState.h"

void GetTextureDop(const char filePath[], mainRenderer* texture, int frameCount, SDL_Renderer* ren)
{
	SDL_Surface* surface = NULL;
	if ((surface = IMG_Load(filePath)) == NULL)
	{
		printf_s("Can't load image '%s'", filePath);
		system("pause");
	}

	texture->texture = SDL_CreateTextureFromSurface(ren, surface);

	texture->textureSize.x = NULL;
	texture->textureSize.y = NULL;
	texture->textureSize.w = surface->w;
	texture->textureSize.h = surface->h;

	texture->frame.x = NULL;
	texture->frame.y = NULL;
	texture->frame.w = surface->w / frameCount;
	texture->frame.h = surface->h;

	texture->angel = NULL;

	texture->frameCount = frameCount;

	SDL_FreeSurface(surface);
}


void MainMenu(GameState* gameState, mainWindow* window, SDL_Renderer * ren, SDL_Window* win) 
{
	mainRenderer startButtonTexture;
	mainRenderer optionsButtonTexture;
	mainRenderer creditsButtonTexture;
	mainRenderer exitButtonTexture;
	GetTextureDop("Textures\\button_start.png", &startButtonTexture, 1, ren);
	GetTextureDop("Textures\\button_options.png", &optionsButtonTexture, 1, ren);
	GetTextureDop("Textures\\button_credits.png", &creditsButtonTexture, 1, ren);
	GetTextureDop("Textures\\button_exit.png", &exitButtonTexture, 1, ren);

	SDL_FRect startButton;
	SDL_FRect settingsButton;
	SDL_FRect creditsButton;
	SDL_FRect exitButton;

	SDL_FRect buttons[4];

	SDL_Event ev;

	while (true)
	{
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		startButton = { float(window->w / 2.3), float(window->h / 3), float(window->w / 8),float(window->h / 8) };
		SDL_RenderCopyF(ren, startButtonTexture.texture, NULL, &startButton);
		buttons[0] = startButton;

		settingsButton = { float(window->w / 2.3),float(window->h / 2.1),float(window->w / 8),  float(window->h / 8) };
		SDL_RenderCopyF(ren, optionsButtonTexture.texture, NULL, &settingsButton);
		buttons[1] = settingsButton;

		creditsButton = { float(window->w / 2.3),float(window->h / 1.6),float(window->w / 8),float(window->h / 8) };
		SDL_RenderCopyF(ren, creditsButtonTexture.texture, NULL, &creditsButton);
		buttons[2] = creditsButton;

		exitButton = { float(window->w / 2.3) ,float(window->h / 1.3),float(window->w / 8),float(window->h / 8) };
		SDL_RenderCopyF(ren, exitButtonTexture.texture, NULL, &exitButton);
		buttons[3] = exitButton;

		SDL_RenderPresent(ren);
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_WINDOWEVENT:
				if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					SDL_GetWindowSize(win, &window->w, &window->h);
				}
				break;
			case SDL_MOUSEBUTTONUP:
			{
				SDL_FPoint mousePoint = { ev.button.x,ev.button.y };
				for (int i = 0; i < 4; i++)
				{
					if (SDL_PointInFRect(&mousePoint, &buttons[i]))
					{
						*gameState = (GameState)i;
						SDL_DestroyTexture(startButtonTexture.texture);
						SDL_DestroyTexture(optionsButtonTexture.texture);
						SDL_DestroyTexture(creditsButtonTexture.texture);
						SDL_DestroyTexture(exitButtonTexture.texture);
						return;
					}
				}
			}
			}
		}
	}
}

void PauseMenu(GameState* gameState, mainWindow* window, SDL_Renderer * ren, SDL_Window* win) 
{

	mainRenderer continueButtonTexture;
	GetTextureDop("Textures\\button_continue.png", &continueButtonTexture, 1, ren);
	mainRenderer exitButtonTexture;
	GetTextureDop("Textures\\button_exit.png", &exitButtonTexture, 1, ren);
	SDL_FRect continueButton;
	SDL_FRect exitButton;


	SDL_Event ev;


	while (true)
	{
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		continueButton = { float(window->w / 2.3), float(window->h / 3.), float(window->w / 8.),float(window->h / 8.) };
		exitButton = { float(window->w / 2.3),float(window->h / 2.1),float(window->w / 8.),  float(window->h / 8.) };
		SDL_RenderCopyF(ren, continueButtonTexture.texture, NULL, &continueButton);
		SDL_RenderCopyF(ren, exitButtonTexture.texture, NULL, &exitButton);

		SDL_RenderPresent(ren);
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_WINDOWEVENT:
				if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					SDL_GetWindowSize(win, &window->w, &window->h);
				}
				break;

			case SDL_MOUSEBUTTONUP:
			{
				SDL_FPoint mousePoint = { ev.button.x,ev.button.y };
				if (SDL_PointInFRect(&mousePoint, &continueButton))
				{
					SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
					SDL_RenderClear(ren);
					SDL_DestroyTexture(exitButtonTexture.texture);
					SDL_DestroyTexture(continueButtonTexture.texture);
					*gameState = IN_GAME;
					return;
				}
				else if (SDL_PointInFRect(&mousePoint, &exitButton))
				{
					SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
					SDL_RenderClear(ren);
					SDL_DestroyTexture(exitButtonTexture.texture);
					SDL_DestroyTexture(continueButtonTexture.texture);
					*gameState = MAIN_MENU;
					return;
				}
			}
			}
		}
	}
}

void SettingsMenu(GameState* gameState, Settings* settings, mainWindow* window, SDL_Renderer* ren, SDL_Window* win) {

	SDL_FRect volumeLeftButton;
	SDL_FRect volumeBar;
	SDL_FRect volumeRightButton;

	mainRenderer plusButtonTexture;
	mainRenderer minusButtonTexture;
	mainRenderer volumeBarTexture;
	mainRenderer ButtonTexture;
	GetTextureDop("Textures\\button_exit.png", &ButtonTexture, 1, ren);
	GetTextureDop("Textures\\batton_plus.png", &plusButtonTexture, 1, ren);
	GetTextureDop("Textures\\button_minus.png", &minusButtonTexture, 1, ren);
	GetTextureDop("Textures\\volume_bar.png", &volumeBarTexture, 1, ren);

	SDL_FRect skinLeftButton;
	SDL_FRect skinRightButton;

	SDL_FRect exitButton;

	SDL_Event ev;

	while (true)
	{
		SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);

		volumeLeftButton = { float(window->w / 3.12), float(window->h / 3.), float(window->w / 25.6), float(window->h / 14.4) };
		volumeRightButton = { float(window->w / 1.66), float(window->h / 3.), float(window->w / 25.6), float(window->h / 14.4) };
		volumeBar = { float(window->w / 2.61),float(window->h / 3.),float(window->w / 25.6) * settings->volume,float(window->h / 14.4) };

		skinLeftButton = { float(window->w / 3.12), float(window->h / 1.63), float(window->w / 25.6), float(window->h / 14.4) };
		skinRightButton = { float(window->w / 1.66),float(window->h / 1.63), float(window->w / 25.6), float(window->h / 14.4) };

		exitButton = { float(window->w / 2.37),float(window->h / 1.2), float(window->w / 8.), float(window->h / 8.) };

		SDL_RenderCopyF(ren, minusButtonTexture.texture, NULL, &volumeLeftButton);
		SDL_RenderCopyF(ren, plusButtonTexture.texture, NULL, &volumeRightButton);

		SDL_RenderCopyF(ren, volumeBarTexture.texture, NULL, &volumeBar);

		SDL_RenderCopyF(ren, minusButtonTexture.texture, NULL, &skinLeftButton);
		SDL_RenderCopyF(ren, plusButtonTexture.texture, NULL, &skinRightButton);

		SDL_RenderCopyF(ren, ButtonTexture.texture, NULL, &exitButton);

		SDL_RenderPresent(ren);



		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_WINDOWEVENT:
				if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					SDL_GetWindowSize(win, &window->w, &window->h);
				}
				break;

			case SDL_MOUSEBUTTONUP:
			{
				SDL_FPoint mousePoint = { ev.button.x,ev.button.y };
				if (SDL_PointInFRect(&mousePoint, &volumeLeftButton) && settings->volume > 0) {
					settings->volume--;
				}
				else if (SDL_PointInFRect(&mousePoint, &volumeRightButton) && settings->volume < 5) {
					settings->volume++;
				}

				if (SDL_PointInFRect(&mousePoint, &skinLeftButton) && settings->skin > 0) {
					settings->skin--;
				}
				else if (SDL_PointInFRect(&mousePoint, &skinRightButton) && settings->skin < 5) {
					settings->skin++;
				}

				if (SDL_PointInFRect(&mousePoint, &exitButton))
				{
					SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
					SDL_RenderClear(ren);
					*gameState = MAIN_MENU;

					SDL_DestroyTexture(ButtonTexture.texture);
					SDL_DestroyTexture(plusButtonTexture.texture);
					SDL_DestroyTexture(minusButtonTexture.texture);
					SDL_DestroyTexture(volumeBarTexture.texture);

					return;
				}
			}
			}
		}

		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);
	}
}

void CreditsMenu(GameState* gameState, mainWindow* window, SDL_Renderer* ren, SDL_Window* win) {

	mainRenderer ButtonTexture;
	GetTextureDop("Textures\\button_exit.png", &ButtonTexture, 1, ren);
	SDL_FRect exitButton;

	SDL_Event ev;

	while (true)
	{
		while (SDL_PollEvent(&ev))
		{
			SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
			SDL_RenderClear(ren);

			exitButton = { float(window->w / 2.37),float(window->h / 1.2), float(window->w / 8), float(window->h / 8) };
			SDL_RenderCopyF(ren, ButtonTexture.texture, NULL, &exitButton);

			SDL_RenderPresent(ren);
			switch (ev.type)
			{
			case SDL_WINDOWEVENT:
				if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					SDL_GetWindowSize(win, &window->w, &window->h);
				}
				break;

			case SDL_MOUSEBUTTONUP:
			{
				SDL_FPoint mousePoint = { ev.button.x,ev.button.y };

				if (SDL_PointInFRect(&mousePoint, &exitButton))
				{
					SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
					SDL_RenderClear(ren);
					SDL_DestroyTexture(ButtonTexture.texture);
					*gameState = MAIN_MENU;
					return;
				}
			}
			}
		}
	}
}