#include "GameState.h"
#include "LoadAndInit.h"
#include <SDL_mixer.h>


void UpSkillLvl(SDL_Point* mousePoint, SDL_Rect button, int* skill_lvl, int* souls)
{
	if (SDL_PointInRect(mousePoint, &button))
		if (*skill_lvl < SKILL_MAX_LVL && *souls >*skill_lvl * SKILL_LVL_POINT_COST)
		{
			(*souls) -= (*skill_lvl) * SKILL_LVL_POINT_COST;
			(*skill_lvl)++;
			*mousePoint = { 0, 0 };
		}
}

void DrawSkillPoints(int* skillLvl, SDL_Rect luckyPoints, SDL_Renderer* ren, SDL_Texture* texture)
{
	for (int i = 0; i < *skillLvl - 1; i++)
	{
		SDL_Rect dopRect = luckyPoints;
		dopRect.x += dopRect.w * i;
		SDL_RenderCopy(ren, texture, NULL, &dopRect);
	}
}

void SkillLeveling(mainHero* Laplas, mainWindow* window, SDL_Renderer* ren, SDL_Window* win, mainTextureSkill* texture_skill,
	TTF_Font* font)
{
	SDL_Rect luckyButton = { int(window->w * (2. / 16.)) , int(window->h * (1. / 9.)), int(window->w * 3 / 16.) ,int(window->h * 3 / 9.) };
	SDL_Rect potionButton = { int(window->w * (6.5 / 16.)), luckyButton.y,   luckyButton.w ,luckyButton.h };
	SDL_Rect healthButton = { int(window->w * (11. / 16.)), luckyButton.y,   luckyButton.w ,luckyButton.h };
	SDL_Rect strengthButton = { luckyButton.x  , int(window->h * (5. / 9.))  , luckyButton.w ,luckyButton.h };
	SDL_Rect ammoButton = { potionButton.x , strengthButton.y , luckyButton.w ,luckyButton.h };
	SDL_Rect weaponButton = { healthButton.x , ammoButton.y , luckyButton.w ,luckyButton.h };

	SDL_Rect luckyPoints = { int(window->w * (2. / 16.)) , int(window->h * (4. / 9.)), int(window->w * 0.5 / 16.) ,int(window->h * 1 / 9.) };
	SDL_Rect potionPoints = { int(window->w * (6.5 / 16.)), luckyPoints.y,   luckyPoints.w ,luckyPoints.h };
	SDL_Rect healthPoints = { int(window->w * (11. / 16.)), luckyPoints.y,   luckyPoints.w ,luckyPoints.h };
	SDL_Rect strengthPoints = { luckyPoints.x  , int(window->h * (8. / 9.))  , luckyPoints.w ,luckyPoints.h };
	SDL_Rect ammoPoints = { potionPoints.x , strengthPoints.y , luckyPoints.w ,luckyPoints.h };
	SDL_Rect weaponPoints = { healthPoints.x , strengthPoints.y , luckyPoints.w ,luckyPoints.h };

	SDL_Event ev;
	SDL_Point mousePoint;
	char souls[5] = { 0 };
	mainRenderer texture;
	texture.texture = 0;

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

					luckyPoints = { int(window->w * (2. / 16.)) , int(window->h * (4. / 9.)), int(window->w * 0.5 / 16.) ,int(window->h * 1 / 9.) };
					potionPoints = { int(window->w * (6.5 / 16.)), luckyPoints.y,   luckyPoints.w ,luckyPoints.h };
					healthPoints = { int(window->w * (11. / 16.)), luckyPoints.y,   luckyPoints.w ,luckyPoints.h };
					strengthPoints = { luckyPoints.x  , int(window->h * (8. / 9.))  , luckyPoints.w ,luckyPoints.h };
					ammoPoints = { potionPoints.x , strengthPoints.y , luckyPoints.w ,luckyPoints.h };
					weaponPoints = { healthPoints.x , strengthPoints.y , luckyPoints.w ,luckyPoints.h };
				}
				break;

			case SDL_KEYUP:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					SDL_DestroyTexture(texture.texture);
					texture.texture = 0;
					return;
					break;
				}
				break;

			case SDL_MOUSEBUTTONUP:
				mousePoint = { ev.button.x,ev.button.y };
				break;
				
			}
		}
		
		UpSkillLvl(&mousePoint, luckyButton, &Laplas->status.lvlLucky, &Laplas->status.souls);
		UpSkillLvl(&mousePoint, potionButton, &Laplas->status.lvlPotion, &Laplas->status.souls);
		UpSkillLvl(&mousePoint, healthButton, &Laplas->status.lvlHealth, &Laplas->status.souls);
		UpSkillLvl(&mousePoint, strengthButton, &Laplas->status.lvlStrength, &Laplas->status.souls);
		UpSkillLvl(&mousePoint, ammoButton, &Laplas->status.lvlAmmo, &Laplas->status.souls);
		UpSkillLvl(&mousePoint, weaponButton, &Laplas->status.lvlWeapon, &Laplas->status.souls);

		sprintf_s(souls, "%d", Laplas->status.souls);

		SDL_Surface* surface = TTF_RenderText_Blended(font, souls, { 25, 229, 230, 255 });
		if (texture.texture != 0)
			SDL_DestroyTexture(texture.texture);


		texture.texture = SDL_CreateTextureFromSurface(ren, surface);
		texture.textureSize.w = surface->w;
		texture.textureSize.h = surface->h;
		texture.textureSize.x = WINDOW_WIDTH - surface->w;
		texture.textureSize.y = NULL;
		SDL_FreeSurface(surface);

		DrawSkillPoints(&Laplas->status.lvlAmmo, ammoPoints, ren, texture_skill->point.texture);
		DrawSkillPoints(&Laplas->status.lvlLucky, luckyPoints, ren, texture_skill->point.texture);
		DrawSkillPoints(&Laplas->status.lvlHealth, healthPoints, ren, texture_skill->point.texture);
		DrawSkillPoints(&Laplas->status.lvlPotion, potionPoints, ren, texture_skill->point.texture);
		DrawSkillPoints(&Laplas->status.lvlStrength, strengthPoints, ren, texture_skill->point.texture);
		DrawSkillPoints(&Laplas->status.lvlWeapon, weaponPoints, ren, texture_skill->point.texture);

		SDL_RenderCopy(ren, texture_skill->iconLucky.texture, NULL, &luckyButton);
		SDL_RenderCopy(ren, texture_skill->iconPotion.texture, NULL, &potionButton);
		SDL_RenderCopy(ren, texture_skill->iconHealth.texture, NULL, &healthButton);
		SDL_RenderCopy(ren, texture_skill->iconStrength.texture, NULL, &strengthButton);
		SDL_RenderCopy(ren, texture_skill->iconAmmo.texture, NULL, &ammoButton);
		SDL_RenderCopy(ren, texture_skill->iconWeapon.texture, NULL, &weaponButton);

		SDL_RenderCopy(ren, texture.texture, NULL, &texture.textureSize);

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

void LoadMenu(GameState* gameState, mainWindow* window, SDL_Renderer* ren, SDL_Window* win, Settings* settings, mainHero* Laplas) {
	if (settings->isLoaded)
	{
		*gameState = IN_GAME;
		return;
	}
		settings->isLoaded = true;

		mainRenderer ButtonTexture;
		GetTextureDop("Textures\\button2.png", &ButtonTexture, 1, ren);
		SDL_FRect loadOneButton;
		SDL_FRect loadTwoButton;
		SDL_FRect loadThreeButton;

		SDL_FRect skinOne;
		SDL_FRect skinTwo;
		SDL_FRect skinThree;

		mainRenderer skinOneTexture;
		mainRenderer skinTwoTexture;
		mainRenderer skinThreeTexture;

		mainHero loadHero1;
		mainHero loadHero2;
		mainHero loadHero3;

		loadLaplas(&loadHero1, "Saves\\1.txt", settings);
		switch (settings->skin) {
		case 1:
			GetTextureDop("Textures\\hero_skin.png", &loadHero1.animation.com, 3, ren);
			break;
		case 2:
			GetTextureDop("Textures\\black_hero_skin.png", &loadHero1.animation.com, 3, ren);
			break;
		case 3:
			GetTextureDop("Textures\\white_hero_skin.png", &loadHero1.animation.com, 3, ren);
			break;
		}
		skinOneTexture.texture = loadHero1.animation.com.texture;


		loadLaplas(&loadHero2, "Saves\\2.txt", settings);
		switch (settings->skin) {
		case 1:
			GetTextureDop("Textures\\hero_skin.png", &loadHero2.animation.com, 3, ren);
			break;
		case 2:
			GetTextureDop("Textures\\black_hero_skin.png", &loadHero2.animation.com, 3, ren);
			break;
		case 3:
			GetTextureDop("Textures\\white_hero_skin.png", &loadHero2.animation.com, 3, ren);
			break;
		}
		skinTwoTexture.texture = loadHero2.animation.com.texture;

		loadLaplas(&loadHero3, "Saves\\3.txt", settings);
		switch (settings->skin) {
		case 1:
			GetTextureDop("Textures\\hero_skin.png", &loadHero3.animation.com, 3, ren);
			break;
		case 2:
			GetTextureDop("Textures\\black_hero_skin.png", &loadHero3.animation.com, 3, ren);
			break;
		case 3:
			GetTextureDop("Textures\\white_hero_skin.png", &loadHero3.animation.com, 3, ren);
			break;
		}
		skinThreeTexture.texture = loadHero3.animation.com.texture;

		SDL_Event ev;

		while (true)
		{
			while (SDL_PollEvent(&ev))
			{
				SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
				SDL_RenderClear(ren);

				loadOneButton = { float(window->w / 12),float(window->h / 4), float(window->w / 4), float(window->h / 2.6) };
				loadTwoButton = { float(window->w / 2.67),float(window->h / 4), float(window->w / 4), float(window->h / 2.6) };
				loadThreeButton = { float(window->w / 1.5),float(window->h / 4), float(window->w / 4), float(window->h / 2.6) };

				skinOne = { float(window->w / 11),float(window->h / 3.6), float(window->w / 5), float(window->h / 3.5) };
				skinTwo = { float(window->w / 2.57),float(window->h / 3.6), float(window->w / 5), float(window->h / 3.5) };
				skinThree = { float(window->w / 1.46),float(window->h / 3.6), float(window->w / 5), float(window->h / 3.5) };

				SDL_RenderCopyF(ren, ButtonTexture.texture, NULL, &loadOneButton);
				SDL_RenderCopyF(ren, ButtonTexture.texture, NULL, &loadTwoButton);
				SDL_RenderCopyF(ren, ButtonTexture.texture, NULL, &loadThreeButton);

				SDL_RenderCopyF(ren, skinOneTexture.texture, NULL, &skinOne);
				SDL_RenderCopyF(ren, skinTwoTexture.texture, NULL, &skinTwo);
				SDL_RenderCopyF(ren, skinThreeTexture.texture, NULL, &skinThree);


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

					if (SDL_PointInFRect(&mousePoint, &loadOneButton))
					{
						settings->loadNum = 1;
						loadLaplas(Laplas, "Saves\\1.txt", settings);
						SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
						SDL_RenderClear(ren);
						SDL_DestroyTexture(ButtonTexture.texture);
						*gameState = IN_GAME;
						return;
					}
					else if (SDL_PointInFRect(&mousePoint, &loadTwoButton))
					{
						settings->loadNum = 2;
						loadLaplas(Laplas, "Saves\\2.txt", settings);
						SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
						SDL_RenderClear(ren);
						SDL_DestroyTexture(ButtonTexture.texture);
						*gameState = IN_GAME;
						return;
					}
					else if (SDL_PointInFRect(&mousePoint, &loadThreeButton))
					{
						settings->loadNum = 3;
						loadLaplas(Laplas, "Saves\\3.txt", settings);
						SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
						SDL_RenderClear(ren);
						SDL_DestroyTexture(ButtonTexture.texture);
						*gameState = IN_GAME;
						return;
					}
				}
				}
			}
		}
}