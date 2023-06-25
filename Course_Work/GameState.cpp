#include "GameState.h"
#include <SDL_mixer.h>


void SkillLeveling(mainHero* Laplas,  mainWindow* window, SDL_Renderer* ren, SDL_Window* win, mainTextureSkill* texture_skill)
{
	SDL_Rect luckyButton = { int(window->w * (2. / 16.)) , int(window->h * (1. / 9.)), int(window->w * 3 / 16.) ,int(window->h * 3 / 9.) };
	SDL_Rect potionButton = { int(window->w * (6.5 / 16.)), luckyButton.y,   luckyButton.w ,luckyButton.h };
	SDL_Rect healthButton = { int(window->w * (11. / 16.)), luckyButton.y,   luckyButton.w ,luckyButton.h };
	SDL_Rect strengthButton = { luckyButton.x  , int(window->h * (5. / 9.))  , luckyButton.w ,luckyButton.h }; 
	SDL_Rect ammoButton = { potionButton.x , strengthButton.y , luckyButton.w ,luckyButton.h };
	SDL_Rect weaponButton = { healthButton.x , ammoButton.y , luckyButton.w ,luckyButton.h };
	
	SDL_Event ev;
	

	while (true)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_WINDOWEVENT:
				if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					SDL_GetWindowSize(win, &window->w, &window->h);
					luckyButton =    { int(window->w * (2. / 16.)) , int(window->h * (1. / 9.)), int(window->w * 3 / 16.) ,int(window->h * 3 / 9.) };
					potionButton =   { int(window->w * (6.5 / 16.)), luckyButton.y,   luckyButton.w ,luckyButton.h };
					healthButton =   { int(window->w * (11. / 16.)), luckyButton.y,   luckyButton.w ,luckyButton.h };
					strengthButton = { luckyButton.x  , int(window->h * (5. / 9.))  , luckyButton.w ,luckyButton.h };
					ammoButton =     { potionButton.x , strengthButton.y , luckyButton.w ,luckyButton.h };
					weaponButton =   { healthButton.x , ammoButton.y , luckyButton.w ,luckyButton.h };
				}
				break;

			case SDL_KEYUP:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					return;
					break;
				}
				break;

			case SDL_MOUSEBUTTONUP:
				SDL_Point mousePoint = { ev.button.x,ev.button.y };
				break;
				
			}
		}
		


		SDL_RenderCopy(ren, texture_skill->iconLucky.texture, NULL, &luckyButton);
		SDL_RenderCopy(ren, texture_skill->iconPotion.texture, NULL, &potionButton);
		SDL_RenderCopy(ren, texture_skill->iconHealth.texture, NULL, &healthButton);
		SDL_RenderCopy(ren, texture_skill->iconStrength.texture, NULL, &strengthButton);
		SDL_RenderCopy(ren, texture_skill->iconAmmo.texture, NULL, &ammoButton);
		SDL_RenderCopy(ren, texture_skill->iconWeapon.texture, NULL, &weaponButton);

		SDL_RenderPresent(ren);

		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);
	}


}

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

void SettingsMenu(GameState* gameState, Settings* settings, mainWindow* window, SDL_Renderer* ren, SDL_Window* win, Audio audio) {

	SDL_FRect musicVolumeLeftButton;
	SDL_FRect musicVolumeRightButton;
	SDL_FRect musicVolumeBar;

	SDL_FRect soundsVolumeLeftButton;
	SDL_FRect soundsVolumeRightButton;
	SDL_FRect soundsVolumeBar;

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
	SDL_FRect skinPreview;
	mainRenderer skinTexture;
	int lastSkin = -1;


	SDL_FRect exitButton;

	SDL_Event ev;

	while (true)
	{
		if (settings->skin != lastSkin)
		{
			switch (settings->skin) {
			case 1:
				GetTextureDop("Textures\\hero_skin.png", &skinTexture, 1, ren);
				break;
			case 2:
				GetTextureDop("Textures\\black_hero_skin.png", &skinTexture, 1, ren);
				break;
			case 3:
				GetTextureDop("Textures\\white_hero_skin.png", &skinTexture, 1, ren);
				break;
			}
			lastSkin = settings->skin;
		}

		SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);

		musicVolumeLeftButton = { float(window->w / 3.12), float(window->h / 3.), float(window->w / 25.6), float(window->h / 14.4) };
		musicVolumeRightButton = { float(window->w / 1.66), float(window->h / 3.), float(window->w / 25.6), float(window->h / 14.4) };
		musicVolumeBar = { float(window->w / 2.61),float(window->h / 3.),float(window->w / 25.6) * settings->musicVolume,float(window->h / 14.4) };

		soundsVolumeLeftButton = { float(window->w / 3.12), float(window->h / 5.), float(window->w / 25.6), float(window->h / 14.4) };
		soundsVolumeRightButton = { float(window->w / 1.66), float(window->h / 5.), float(window->w / 25.6), float(window->h / 14.4) };
		soundsVolumeBar = { float(window->w / 2.61),float(window->h / 5.),float(window->w / 25.6) * settings->soundsVolume,float(window->h / 14.4) };

		skinLeftButton = { float(window->w / 3.12), float(window->h / 1.63), float(window->w / 25.6), float(window->h / 14.4) };
		skinRightButton = { float(window->w / 1.66),float(window->h / 1.63), float(window->w / 25.6), float(window->h / 14.4) };
		skinPreview = { float(window->w/2 - skinTexture.textureSize.w/1.5),float(window->h / 1.8), float(skinTexture.textureSize.w), float(skinTexture.textureSize.h) };

		exitButton = { float(window->w / 2.37),float(window->h / 1.2), float(window->w / 8.), float(window->h / 8.) };

		

		SDL_RenderCopyF(ren, minusButtonTexture.texture, NULL, &musicVolumeLeftButton);
		SDL_RenderCopyF(ren, plusButtonTexture.texture, NULL, &musicVolumeRightButton);
		SDL_RenderCopyF(ren, volumeBarTexture.texture, NULL, &musicVolumeBar);

		SDL_RenderCopyF(ren, minusButtonTexture.texture, NULL, &soundsVolumeLeftButton);
		SDL_RenderCopyF(ren, plusButtonTexture.texture, NULL, &soundsVolumeRightButton);
		SDL_RenderCopyF(ren, volumeBarTexture.texture, NULL, &soundsVolumeBar);

		SDL_RenderCopyF(ren, minusButtonTexture.texture, NULL, &skinLeftButton);
		SDL_RenderCopyF(ren, plusButtonTexture.texture, NULL, &skinRightButton);
		SDL_RenderCopyF(ren, skinTexture.texture, NULL, &skinPreview);


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
				if (SDL_PointInFRect(&mousePoint, &musicVolumeLeftButton) && settings->musicVolume > 0) {
					settings->musicVolume--;
					Mix_VolumeMusic(255 * settings->musicVolume / 5);
				}
				else if (SDL_PointInFRect(&mousePoint, &musicVolumeRightButton) && settings->musicVolume < 5) {
					settings->musicVolume++;
					Mix_VolumeMusic(255 * settings->musicVolume / 5);
				}

				if (SDL_PointInFRect(&mousePoint, &soundsVolumeLeftButton) && settings->soundsVolume > 0) {
					settings->soundsVolume--;
				}
				else if (SDL_PointInFRect(&mousePoint, &soundsVolumeRightButton) && settings->soundsVolume < 5) {
					settings->soundsVolume++;
				}

				if (SDL_PointInFRect(&mousePoint, &skinLeftButton) && settings->skin > 1) {
					settings->skin--;
				}
				else if (SDL_PointInFRect(&mousePoint, &skinRightButton) && settings->skin < 3) {
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
					SDL_DestroyTexture(skinTexture.texture);

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