#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include "func.h"
#include "common_parameters.h"
#include <math.h>
#include <time.h>
#include "HeroPhysic.h"
#include "EnemyPhysic.h"
#include "HeroBattle.h"
#include "Draw.h"
#include "LoadAndInit.h"
#include "GameState.h"
#include "GameLogic.h"
#include "Audio.h"

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
SDL_Surface* win_surface = NULL;
SDL_Event ev;
Audio audio = {NULL,NULL, false};

void GetTexture(const char filePath[], mainRenderer* texture, int frameCount, SDL_Renderer* ren)
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

	texture->frameTime = 0;

	SDL_FreeSurface(surface);
}




mainRenderer CreateAmmunitionFromText(const char str[], TTF_Font* font, SDL_Color fg)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, str, fg);
	mainRenderer texture;
	texture.texture = SDL_CreateTextureFromSurface(ren, surface);
	texture.textureSize.w = surface->w;
	texture.textureSize.h = surface->h;
	texture.textureSize.x = WINDOW_WIDTH/8;
	texture.textureSize.y = WINDOW_HEIGHT - WINDOW_HEIGHT/9;
	SDL_FreeSurface(surface);
	return texture;
}

mainRenderer CreateTimerFromText(const char str[], TTF_Font* font, SDL_Color fg, SDL_Renderer* ren)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, str, fg);
	mainRenderer texture;
	texture.texture = SDL_CreateTextureFromSurface(ren, surface);
	texture.textureSize.w = surface->w;
	texture.textureSize.h = surface->h;
	texture.textureSize.x = WINDOW_WIDTH - surface->w;
	texture.textureSize.y = NULL;
	SDL_FreeSurface(surface);
	return texture;
}

void LoadMap(mainMap* map, const char mapName[])
{
	FILE* f;
	if(fopen_s(&f, mapName, "r") != 0)
	{
		printf_s("Error map\n");
		system("pause");
	}

	fscanf_s(f, "%d", &map->mapSize.i);
	fscanf_s(f, "%d", &map->mapSize.j);

	map->map = (int**)malloc(sizeof(int*) * (map->mapSize.i));
	for (int i = 0; i < map->mapSize.i; i++)
		map->map[i] = (int*)malloc(sizeof(int) * (map->mapSize.j));

	map->borders = (mainBorders***)malloc(sizeof(mainBorders**) * map->mapSize.i);
	map->enemys = (mainEnemys***)malloc(sizeof(mainEnemys**) * map->mapSize.i);
	map->traps = (mainTraps***)malloc(sizeof(mainTraps**) * map->mapSize.i);
	map->items = (mainItems***)malloc(sizeof(mainItems**) * map->mapSize.i);

	map->bordersCount = (int**)malloc(sizeof(int*) * (map->mapSize.i));
	for (int i = 0; i < map->mapSize.i; i++)
		map->bordersCount[i] = (int*)malloc(sizeof(int) * (map->mapSize.j));

	map->itemsCount = (int**)malloc(sizeof(int*) * (map->mapSize.i));
	for (int i = 0; i < map->mapSize.i; i++)
		map->itemsCount[i] = (int*)malloc(sizeof(int) * (map->mapSize.j));

	map->enemysCount = (int**)malloc(sizeof(int*) * (map->mapSize.i));
	for (int i = 0; i < map->mapSize.i; i++)
		map->enemysCount[i] = (int*)malloc(sizeof(int) * (map->mapSize.j));

	map->trapsCount = (int**)malloc(sizeof(int*) * (map->mapSize.i));
	for (int i = 0; i < map->mapSize.i; i++)
		map->trapsCount[i] = (int*)malloc(sizeof(int) * (map->mapSize.j));

	map->levelSize = (twoParam**)malloc(sizeof(twoParam*) * map->mapSize.i);
	for (int i = 0; i < map->mapSize.i; i++)
		map->levelSize[i] = (twoParam*)malloc(sizeof(twoParam) * (map->mapSize.j));

	for (int i = 0; i < map->mapSize.i; i++)
	{
		map->borders[i] = (mainBorders**)malloc(sizeof(mainBorders*) * (map->mapSize.j));
		map->enemys[i] = (mainEnemys**)malloc(sizeof(mainEnemys*) * (map->mapSize.j));
		map->traps[i] = (mainTraps**)malloc(sizeof(mainTraps*) * (map->mapSize.j));
		map->items[i] = (mainItems**)malloc(sizeof(mainItems*) * (map->mapSize.j));
		for (int j = 0; j < map->mapSize.j; j++)
		{
			fscanf_s(f, "%d", &map->map[i][j]);
		}
	}

	fclose(f);
}

void InitMap(mainMap* map, mainHero* Laplas, mainRenderer* texture_beaver_run, mainRenderer* texture_beaver_atack,
	mainRenderer* texture_beaver_preAtack, mainRenderer* texture_krab_run, mainRenderer* texture_acid_effect,
	mainRenderer* texture_tower, mainRenderer* texture_tower_bullet, mainRenderer* texture_digit_idle, mainRenderer* texture_digit_atack,
	mainRenderer* texture_barrel, mainRenderer* texture_dart_trap, mainRenderer* texture_pressure_plate, mainRenderer* texture_trap_spikes,
	mainRenderer* texture_buff_DMG, mainRenderer* texture_item_Rubber_Bullet, mainRenderer* texture_item_Ball,
	mainRenderer* texture_item_acid, mainRenderer* texture_buff_speed, mainRenderer* texture_buff_lucky,
	mainRenderer* texture_skill_figure, mainRenderer* texture_kebab, mainRenderer* texture_shop, 
	mainRenderer* texture_bat_with_anvil, mainRenderer* texture_bat_without_anvil, mainRenderer* texture_anvil_without_bat)
{
	char itemPath[] = "Items\\Item00.txt";
	char bordersPath[] = "Levels\\Borders00.txt";
	char trapsPath[] = "Traps\\Trap00.txt";
	char enemysPath[] = "Enemys\\Enemy00.txt";

	for (int i = 0; i < map->mapSize.i; i++)
		for (int j = 0; j < map->mapSize.j; j++)
			if (map->map[i][j] != 0)
			{
				if (map->map[i][j] == 99)
					Laplas->curRoom = { i, j };

				itemPath[10] = map->map[i][j] / 10 + '0';
				itemPath[11] = map->map[i][j] % 10 + '0';

				bordersPath[14] = map->map[i][j] / 10 + '0';
				bordersPath[15] = map->map[i][j] % 10 + '0';

				trapsPath[10] = map->map[i][j] / 10 + '0';
				trapsPath[11] = map->map[i][j] % 10 + '0';

				enemysPath[12] = map->map[i][j] / 10 + '0';
				enemysPath[13] = map->map[i][j] % 10 + '0';

				map->borders[i][j] = LoadLevel(map->borders[i][j], &map->bordersCount[i][j], Laplas, bordersPath, &map->levelSize[i][j].i, &map->levelSize[i][j].j);

				map->enemys[i][j] = LoadEnemys(map->enemys[i][j], &map->enemysCount[i][j], enemysPath);
				InitEnemys(map->enemys[i][j], &map->enemysCount[i][j], texture_beaver_run, texture_beaver_atack, texture_beaver_preAtack,
					texture_krab_run, texture_acid_effect, texture_tower, texture_tower_bullet,
					texture_digit_idle, texture_digit_atack, texture_bat_with_anvil, texture_bat_without_anvil,
					texture_anvil_without_bat, texture_barrel);

				map->traps[i][j] = LoadTraps(map->traps[i][j], &map->trapsCount[i][j], trapsPath);
				InitTraps(map->traps[i][j], &map->trapsCount[i][j], texture_dart_trap, texture_pressure_plate,
					texture_trap_spikes);

				map->items[i][j] = LoadItems(map->items[i][j], &map->itemsCount[i][j], itemPath);
				InitItems(map->items[i][j], &map->itemsCount[i][j], texture_buff_DMG, texture_item_Rubber_Bullet, texture_barrel,
					texture_item_Ball, texture_item_acid, texture_buff_speed, texture_buff_lucky, texture_skill_figure, texture_kebab,
					texture_shop);

			}
}

void FreeMap(mainMap* map)
{
	for (int i = 0; i < map->mapSize.i; i++)
	{

		
		
		for (int j = 0; j < map->mapSize.j; j++)
		{
			if (map->map[i][j] != 0)
			{
				free(map->borders[i][j]);
				free(map->enemys[i][j]);
				free(map->traps[i][j]);
				free(map->items[i][j]);
			}
		}
		
		free(map->map[i]);
		free(map->bordersCount[i]);
		free(map->itemsCount[i]);
		free(map->enemysCount[i]);
		free(map->trapsCount[i]);

		free(map->borders[i]);
		free(map->enemys[i]);
		free(map->traps[i]);
		free(map->items[i]);
		free(map->levelSize[i]);
	}
	free(map->map);
	free(map->bordersCount);
	free(map->itemsCount);
	free(map->enemysCount);
	free(map->trapsCount);
	free(map->borders);
	free(map->enemys);
	free(map->traps);
	free(map->items);
	free(map->levelSize);
}

int main(int argc, char* argv[])
{
	int seed = 1002139123;
	srand(seed);
	bool flag = 1;
	Init(&win, &ren, &win_surface);

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);

	#pragma region VARIABLE_INIT

	GameState gameState = MAIN_MENU;
	Settings settings = { 1, 5, 1 };
	int tmp;
	mainMap map;

	mainRenderer texture_backGround;
	mainRenderer texture_cobbleStone;
	mainRenderer texture_cobbleStone_fake;
	mainRenderer texture_hpBar;
	mainRenderer texture_timer;
	mainRenderer texture_ammunition;
	mainRenderer texture_beaver_run;
	mainRenderer texture_beaver_preatack;
	mainRenderer texture_beaver_atack;
	mainRenderer texture_krab;
	mainRenderer texture_platform;
	mainRenderer texture_tmp_platform;
	mainRenderer texture_trampline;
	mainRenderer texture_trap_with_dart;
	mainRenderer texture_pressure_plate;
	mainRenderer texture_trap_dart;
	mainRenderer texture_trap_spikes;
	mainRenderer texture_overheating;
	mainRenderer texture_dark;
	mainRenderer texture_buff_DMG;
	mainRenderer texture_item_Rubber_Bullet;
	mainRenderer texture_item_acid;
	mainRenderer texture_acid_effect;
	mainRenderer texture_tower;
	mainRenderer texture_tower_bullet;
	mainRenderer texture_digit_atack;
	mainRenderer texture_digit_idle;
	mainRenderer texture_buff_lucky;
	mainRenderer texture_buff_speed;
	mainRenderer texture_kebab;
	mainRenderer texture_skill_figure;
	mainRenderer texture_skill_point;
	mainRenderer texture_shop;

	mainRenderer texture_bat_with_anvil;
	mainRenderer texture_bat_without_anvil;
	mainRenderer texture_anvil_without_bat;

	mainRenderer texture_item_Ball;
	mainRenderer texture_barrel;
	TTF_Font* fontNovemBig = NULL;
	TTF_Font* fontNovemSmall = NULL;

	
	mainRenderer hpBarTexture;
	mainRenderer hpBarEdgingTexture;
	mainRenderer enemyHpBarEdgingTexture;
	mainRenderer ammoBarTexture;

	int lastTime = 0;
	int temp = 0;
	int timeInGame = clock();
	int check = 1;

	mainHero Laplas = {0};
	mainTextureSkill texture_skill;
	
	static mainWindow window = { WINDOW_WIDTH ,WINDOW_HEIGHT };
	SDL_Point mouseClick = { NULL, NULL };
	
	char timer_text[10] = "00:00";
	char amunition_text[10] = "000";
	

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	const char main_music[] = "Sounds\\main_theme.wav";
	const char walk_sound[] = "Sounds\\walk.mp3";
	const char punch_sound[] = "Sounds\\punch.mp3";
	const char shot_sound[] = "Sounds\\shot.mp3";

	bool isRunning;

	#pragma endregion

	
	InitHero(&Laplas);
	loadLaplas(&Laplas, "Saves\\1.txt", &settings);
	dopInitHero(&Laplas);

	#pragma region TEXTURES_LOAD

	switch (settings.skin) {
	case 1:
		GetTexture("Textures\\hero_comm.png", &Laplas.animation.com, 3, ren);
		GetTexture("Textures\\hero_run.png", &Laplas.animation.run, 8, ren);
		GetTexture("Textures\\hero_atack3.png", &Laplas.animation.punch, 5, ren);
		GetTexture("Textures\\hero_shoot.png", &Laplas.animation.shoot, 3, ren);
		break;
	case 2:
		GetTexture("Textures\\white_hero_shoot.png", &Laplas.animation.shoot, 3, ren);
		GetTexture("Textures\\black_hero_comm.png", &Laplas.animation.com, 3, ren);
		GetTexture("Textures\\black_hero_run.png", &Laplas.animation.run, 8, ren);
		break;
	case 3:
		GetTexture("Textures\\white_hero_comm.png", &Laplas.animation.com, 3, ren);
		GetTexture("Textures\\white_hero_run.png", &Laplas.animation.run, 8, ren);
		GetTexture("Textures\\white_hero_atack3.png", &Laplas.animation.punch, 5, ren);
		break;
	}

	
	
	
	
	GetTexture("Textures\\hero_bullet.png", &Laplas.animation.bullet, 1, ren);
	GetTexture("Textures\\hero_rubber_bullet.png", &Laplas.animation.rubber_bullet, 1, ren);
	GetTexture("Textures\\ball.png", &Laplas.animation.ball, 1, ren);
	GetTexture("Textures\\overheating.png", &Laplas.animation.DMG_Buff, 1, ren);
	GetTexture("Textures\\dark.png", &Laplas.animation.dark, 1, ren);
	Laplas.texture_rect.w = Laplas.texture_rect.h = Laplas.hitbox.h / 22. * 32;

	GetTexture("Textures\\bobr_run.png", &texture_beaver_run, 6, ren);
	GetTexture("Textures\\bobr_preatack.png", &texture_beaver_preatack, 1, ren);
	GetTexture("Textures\\bobr_atack.png", &texture_beaver_atack, 2, ren);

	GetTexture("Textures\\barrel.png", &texture_barrel, 1, ren);

	GetTexture("Textures\\texture_digit_atack.png", &texture_digit_atack, 7, ren);
	GetTexture("Textures\\texture_digit_idle.png", &texture_digit_idle, 1, ren);

	GetTexture("Textures\\krab.png", &texture_krab, 3, ren);

	GetTexture("Textures\\BackGroundCave.png", &texture_backGround, 1, ren);

	GetTexture("Textures\\cobblestone40x40.png", &texture_cobbleStone, 1, ren);
	GetTexture("Textures\\cobblestone20x20.png", &texture_cobbleStone_fake, 1, ren);

	GetTexture("Textures\\woodenPlatform.png", &texture_platform, 1, ren);

	GetTexture("Textures\\tmp_platform.png", &texture_tmp_platform, 1, ren);
	
	GetTexture("Textures\\trap_with_dart.png", &texture_trap_with_dart, 1, ren);

	GetTexture("Textures\\pressure_plate.png", &texture_pressure_plate, 1, ren);

	GetTexture("Textures\\trap_dart.png", &texture_trap_dart, 1, ren);

	GetTexture("Textures\\trap_spikes.png", &texture_trap_spikes, 1, ren);

	GetTexture("Textures\\trampline.png", &texture_trampline, 1, ren);

	GetTexture("Textures\\buff_Rubber_Bullet.png", &texture_item_Rubber_Bullet, 36, ren);

	GetTexture("Textures\\bat_with_anvil.png", &texture_bat_with_anvil, 3, ren);
	GetTexture("Textures\\bat_without_anvil.png", &texture_bat_without_anvil, 3, ren);
	GetTexture("Textures\\anvil_without_bat.png", &texture_anvil_without_bat, 1, ren);

	GetTexture("Textures\\acid.png", &texture_item_acid, 29, ren);

	texture_item_Ball = Laplas.animation.ball;

	GetTexture("Textures\\buff_DMG.png", &texture_buff_DMG, 1, ren);
	GetTexture("Textures\\potion_speed.png", &texture_buff_speed, 1, ren);
	GetTexture("Textures\\potion_lucky.png", &texture_buff_lucky, 1, ren);
	GetTexture("Textures\\kebab.png", &texture_kebab, 1, ren);
	GetTexture("Textures\\shop.png", &texture_shop, 1, ren);

	

	GetTexture("Textures\\life_bar.png", &hpBarTexture, 1, ren);
	GetTexture("Textures\\outside_life_bar.png", &hpBarEdgingTexture, 1, ren);
	GetTexture("Textures\\ammo_bar.png", &ammoBarTexture, 1, ren);

	GetTexture("Textures\\life_bar.png", &hpBarTexture, 1, ren);
	GetTexture("Textures\\outside_life_bar.png", &hpBarEdgingTexture, 1, ren);
	GetTexture("Textures\\enemy_outside_life_bar.png", &enemyHpBarEdgingTexture, 1, ren);
	GetTexture("Textures\\ammo_bar.png", &ammoBarTexture, 1, ren);
	GetTexture("Textures\\life_bar.png", &hpBarTexture, 1, ren);
	GetTexture("Textures\\outside_life_bar.png", &hpBarEdgingTexture, 1, ren);
	GetTexture("Textures\\ammo_bar.png", &ammoBarTexture, 1, ren);
	GetTexture("Textures\\ammo_bar.png", &ammoBarTexture, 1, ren);

	GetTexture("Textures\\acid_effect.png", &texture_acid_effect, 5, ren);

	GetTexture("Textures\\tower.png", &texture_tower, 1, ren);
	GetTexture("Textures\\tower_bullet.png", &texture_tower_bullet, 1, ren);

	GetTexture("Textures\\skill_potion.png", &texture_skill.iconPotion, 1, ren);
	GetTexture("Textures\\skill_lucky.png", &texture_skill.iconLucky, 1, ren);
	GetTexture("Textures\\skill_gun.png", &texture_skill.iconWeapon, 1, ren);
	GetTexture("Textures\\skill_strength.png", &texture_skill.iconStrength, 1, ren);
	GetTexture("Textures\\skill_health.png", &texture_skill.iconHealth, 1, ren);
	GetTexture("Textures\\skill_patron.png", &texture_skill.iconAmmo, 1, ren);
	GetTexture("Textures\\skill_figure.png", &texture_skill.figure, 1, ren);
	texture_skill_figure = texture_skill.figure;
	GetTexture("Textures\\skill_point.png", &texture_skill.point, 1, ren);
	texture_skill_point = texture_skill.point;

	#pragma endregion

	#pragma region TIMER_TEXTURE

	SDL_Surface* surface = NULL;
	if ((fontNovemBig = TTF_OpenFont("Fonts\\novem.ttf", TIMER_TEXT_SIZE)) == NULL)
	{
		printf_s("Can't open 'novem.ttf'\n");
		system("pause");
	}

	surface = TTF_RenderText_Blended(fontNovemBig, timer_text, { 100, 100, 100, 255 });

	texture_timer.texture = SDL_CreateTextureFromSurface(ren, surface);
	texture_timer.textureSize.w = surface->w;
	texture_timer.textureSize.h = surface->h;
	texture_timer.textureSize.x = WINDOW_WIDTH - surface->w;
	texture_timer.textureSize.y = NULL;

	SDL_FreeSurface(surface);

	#pragma endregion

	#pragma region AMMUNITION_TEXTURE

	if ((fontNovemSmall = TTF_OpenFont("Fonts\\novem.ttf", AMMUNITION_TEXT_SIZE)) == NULL)
	{
		printf_s("Can't open 'novem.ttf'\n");
		system("pause");
	}

	surface = NULL;

	surface = TTF_RenderText_Blended(fontNovemSmall, timer_text, { 255, 215, 0, 255 });

	texture_ammunition.texture = SDL_CreateTextureFromSurface(ren, surface);
	texture_ammunition.textureSize.w = surface->w;
	texture_ammunition.textureSize.h = surface->h;
	texture_ammunition.textureSize.x = NULL;
	texture_ammunition.textureSize.y = NULL;

	SDL_FreeSurface(surface);

	#pragma endregion

	LoadMap(&map, "map.txt");

	InitMap(&map, &Laplas, &texture_beaver_run, &texture_beaver_atack, &texture_beaver_preatack, &texture_krab,
		&texture_acid_effect, &texture_tower, &texture_tower_bullet, &texture_digit_idle, &texture_digit_atack,
		&texture_barrel, &texture_trap_with_dart, &texture_pressure_plate, &texture_trap_spikes,
		&texture_buff_DMG, &texture_item_Rubber_Bullet, &texture_item_Ball, &texture_item_acid, &texture_buff_speed,
		&texture_buff_lucky, &texture_skill_figure, &texture_kebab, &texture_shop, &texture_bat_with_anvil, &texture_bat_without_anvil, &texture_anvil_without_bat);

	while (flag)
	{
		
		switch (gameState)
		{
		case MAIN_MENU:
			PlayMusic(main_music,&audio,settings.musicVolume);
			MainMenu(&gameState, &window, ren, win);
			break;

		case IN_GAME:
			isRunning = true;
			SDL_DestroyTexture(Laplas.animation.com.texture);
			SDL_DestroyTexture(Laplas.animation.run.texture);
			SDL_DestroyTexture(Laplas.animation.punch.texture);
			SDL_DestroyTexture(Laplas.animation.shoot.texture);
			switch (settings.skin) {
			case 1:
				GetTexture("Textures\\hero_comm.png", &Laplas.animation.com, 3, ren);
				GetTexture("Textures\\hero_run.png", &Laplas.animation.run, 8, ren);
				GetTexture("Textures\\hero_atack3.png", &Laplas.animation.punch, 5, ren);
				GetTexture("Textures\\hero_shoot.png", &Laplas.animation.shoot, 3, ren);
				break;
			case 2:
				GetTexture("Textures\\black_hero_atack3.png", &Laplas.animation.punch, 5, ren);
				GetTexture("Textures\\black_hero_comm.png", &Laplas.animation.com, 3, ren);
				GetTexture("Textures\\black_hero_run.png", &Laplas.animation.run, 8, ren);
				GetTexture("Textures\\black_hero_shoot.png", &Laplas.animation.shoot, 3, ren);
				break;
			case 3:
				GetTexture("Textures\\white_hero_comm.png", &Laplas.animation.com, 3, ren);
				GetTexture("Textures\\white_hero_run.png", &Laplas.animation.run, 8, ren);
				GetTexture("Textures\\white_hero_atack3.png", &Laplas.animation.punch, 5, ren);
				GetTexture("Textures\\white_hero_shoot.png", &Laplas.animation.shoot, 3, ren);
				break;
			}



			while (isRunning)
			{
				timeInGame += 17;
			

				#pragma region BUTTON_CHECK
				while (SDL_PollEvent(&ev))
				{
					switch (ev.type)
					{
					case SDL_QUIT:
						isRunning = false;
						break;

					case SDL_WINDOWEVENT:
						switch (ev.window.event)
						{
						case SDL_WINDOWEVENT_RESIZED:
							SDL_GetWindowSize(win, &window.w, &window.h);
							SDL_RenderSetScale(ren, window.scaleX = window.w / 1. / WINDOW_WIDTH, window.scaleY = window.h / 1. / WINDOW_HEIGHT);
							window.w /= window.scaleX;
							window.h /= window.scaleY;

							break;
						}
						break;

					case SDL_KEYDOWN:
						switch (ev.key.keysym.scancode)
						{
						case SDL_SCANCODE_ESCAPE:
							isRunning = false;
							gameState = PAUSE_MENU;
							Laplas.keys.pressed_E = 0;
							Laplas.physic.xMoveL = 0;
							Laplas.physic.xMoveR = 0;
							StopSound(&audio, 1);
							break;

						case SDL_SCANCODE_A:
							Laplas.physic.xMoveL = -1;
							PlaySound(walk_sound, 1, &audio, settings.soundsVolume, -1);
							break;

						case SDL_SCANCODE_D:
							Laplas.physic.xMoveR = 1;
							PlaySound(walk_sound, 1, &audio, settings.soundsVolume, -1);
							break;

						case SDL_SCANCODE_S:
							Laplas.keys.pressed_S = 1;
							break;

						case SDL_SCANCODE_E:
							Laplas.keys.pressed_E = 1;
							break;


						case SDL_SCANCODE_LSHIFT:
							if (timeInGame - Laplas.effect.timeDashCD > Laplas.effect.dashCD)
							{
								Laplas.effect.timeDashCD = timeInGame;
								Laplas.physic.accelerationX = 8;
							}
							break;

						case SDL_SCANCODE_SPACE:
							if (Laplas.physic.onBorder)
							{
								Laplas.physic.impulse = 0.8;
								Laplas.physic.accelerationY = 0;
								Laplas.keys.pressed_Space = 1;
							}
							break;
						}
						break;



					case SDL_KEYUP:
						switch (ev.key.keysym.scancode)
						{
						case SDL_SCANCODE_A:
							Laplas.physic.xMoveL = 0;
							StopSound(&audio, 1);
							break;

						case SDL_SCANCODE_S:
							Laplas.keys.pressed_S = 0;
							break;

						case SDL_SCANCODE_D:
							Laplas.physic.xMoveR = 0;
							StopSound(&audio, 1);
							break;

						case SDL_SCANCODE_E:
							Laplas.keys.pressed_E = 0;
							break;

						case SDL_SCANCODE_SPACE:
							Laplas.keys.pressed_Space = 0;
							break;

						}
						break;

					case SDL_MOUSEBUTTONDOWN:
						if (ev.button.button == SDL_BUTTON_X2)
						{
							if (timeInGame - Laplas.effect.timeDashCD > Laplas.effect.dashCD)
							{
								Laplas.effect.timeDashCD = timeInGame;
								Laplas.physic.accelerationX = 8;
							}
						}
						break;


					case SDL_MOUSEBUTTONUP:
						if (ev.button.button == SDL_BUTTON_LEFT)
						{
							if (!Laplas.battle.commonAtack && !Laplas.battle.shootAtack)
							{
								Laplas.effect.timeAtackCD = timeInGame;
								Laplas.battle.commonAtack = 1;
								EndSound(&audio);
								PlaySound(punch_sound, 1, &audio,settings.soundsVolume, 0);
								
							}
						}
						else if (ev.button.button == SDL_BUTTON_RIGHT)
						{
							if (Laplas.status.ammunition > 0 && !Laplas.battle.shootAtack && !Laplas.battle.commonAtack)
							{
								Laplas.effect.timeShootCD = timeInGame;
								Laplas.battle.shootAtack = 1;
								Laplas.status.ammunition -= 1;
								AddNewBullet(&Laplas);
								PlaySound(shot_sound, 1, &audio, settings.soundsVolume, 0);
							}
						}
						break;
					}



				}
				#pragma endregion

				#pragma region PHYSIC_CHECK

				//Получение координат
				HeroPhysicGetBase(&Laplas);
				EnemyPhysicGetBase(map.enemys[Laplas.curRoom.i][Laplas.curRoom.j], &map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j]);

				//Движение по оси X + рывок
				if(Laplas.animationType != 2 && Laplas.animationType != 3)
					HeroPhysicXmovement(&Laplas);

				//Прыжок
				HeroPhysicJump(&Laplas);
				EnemyPhysicJump(map.enemys[Laplas.curRoom.i][Laplas.curRoom.j], &map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j], &Laplas);

				//Гравитация
				HeroPhysicGravity(&Laplas);
				EnemyPhysicGravity(map.enemys[Laplas.curRoom.i][Laplas.curRoom.j], &map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j]);

				//Проверка коллизии
				HeroPhysicHitboxOverlay(&map.bordersCount[Laplas.curRoom.i][Laplas.curRoom.j], &Laplas, map.borders[Laplas.curRoom.i][Laplas.curRoom.j], &map.trapsCount[Laplas.curRoom.i][Laplas.curRoom.j], map.traps[Laplas.curRoom.i][Laplas.curRoom.j], timeInGame); 
				EnemyPhysicHitboxOverlay(&map.bordersCount[Laplas.curRoom.i][Laplas.curRoom.j], &map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j], map.enemys[Laplas.curRoom.i][Laplas.curRoom.j], map.borders[Laplas.curRoom.i][Laplas.curRoom.j], timeInGame, &Laplas); 

				//Проверка пуль на касание с стеной
				TrapBulletHitboxInRange(map.traps[Laplas.curRoom.i][Laplas.curRoom.j], &map.trapsCount[Laplas.curRoom.i][Laplas.curRoom.j], &map.bordersCount[Laplas.curRoom.i][Laplas.curRoom.j], map.borders[Laplas.curRoom.i][Laplas.curRoom.j]);
				HeroBulletHitboxInRange(&Laplas, &map.bordersCount[Laplas.curRoom.i][Laplas.curRoom.j], map.borders[Laplas.curRoom.i][Laplas.curRoom.j]);
				EnemysBulletHitboxInRange(map.enemys[Laplas.curRoom.i][Laplas.curRoom.j], &map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j], &map.bordersCount[Laplas.curRoom.i][Laplas.curRoom.j], map.borders[Laplas.curRoom.i][Laplas.curRoom.j]);

				//Выход за границы мира
				HeroPhysicOutworldCheck(&Laplas, map.borders[Laplas.curRoom.i][Laplas.curRoom.j]);
				EnemyPhysicOutworldCheck(&map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j], map.enemys[Laplas.curRoom.i][Laplas.curRoom.j], map.borders[Laplas.curRoom.i][Laplas.curRoom.j]);
				HeroBulletOutworldCheck(&Laplas, map.borders[Laplas.curRoom.i][Laplas.curRoom.j]);
				TrapBulletOutworldCheck(map.traps[Laplas.curRoom.i][Laplas.curRoom.j], &map.trapsCount[Laplas.curRoom.i][Laplas.curRoom.j], map.borders[Laplas.curRoom.i][Laplas.curRoom.j]);

				#pragma endregion 

				#pragma region LOGIC_CHECK

				//Подбор предмета
				ItemEquip(&Laplas, map.items[Laplas.curRoom.i][Laplas.curRoom.j], &map.itemsCount[Laplas.curRoom.i][Laplas.curRoom.j],timeInGame);


				//Проверка таймера зелий
				BuffsStateCheck(&Laplas, timeInGame);

				//Срабатывание триггера обнаружения врагом
				EnemyTrigger(map.enemys[Laplas.curRoom.i][Laplas.curRoom.j], &Laplas, &map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j]);

				//Движение врагов
				EnemysMovement(&map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j], map.enemys[Laplas.curRoom.i][Laplas.curRoom.j], &Laplas);

				//Проверка статуса врага
				for(int i = 0; i < map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j]; i++)
					if(map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][i].status.alive && map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][i].effect.poisoned && map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][i].status.HP > 2)
						if (map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][i].effect.poisonLastDamage + POISON_CD < timeInGame)
						{
							map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][i].effect.poisonLastDamage = timeInGame;
							map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][i].status.HP -= POISON_DMG;
						}

				//Анимация атаки
				for (int i = 0; i < map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j]; i++)
					if (map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][i].type != 6)
					{
						if (map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][i].hitbox.x > Laplas.hitbox.x - map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][i].hitbox.w / 1.25 && map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][i].hitbox.x < Laplas.hitbox.x + map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][i].hitbox.w / 1.25)
							map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][i].animation_type = 6;
						else
							map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][i].animation_type = 1;
					}

				//Переход на другую локацию
				if(Laplas.lastLocalSwap + LOCAL_SWAP_CD < timeInGame)
					for (int i = 0; i < map.bordersCount[Laplas.curRoom.i][Laplas.curRoom.j]; i++)
					{
						switch(map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].type)
						{
						case 11:
							if (HeroPhysicInRange({ Laplas.hitbox.x, Laplas.hitbox.y }, map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].bordersHitbox))
							{
								Laplas.lastLocalSwap = timeInGame;
								Laplas.curRoom.i--;
								for(int i = 0; i < map.bordersCount[Laplas.curRoom.i][Laplas.curRoom.j]; i++)
									if (map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].type == 14)
									{
										Laplas.hitbox.x = map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].bordersHitbox.x;
										Laplas.hitbox.y = map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].bordersHitbox.y;
									}
							}
							break;
						
						case 12:
							if (HeroPhysicInRange({ Laplas.hitbox.x, Laplas.hitbox.y }, map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].bordersHitbox))
							{
								Laplas.lastLocalSwap = timeInGame;
								Laplas.curRoom.j--;
								for (int i = 0; i < map.bordersCount[Laplas.curRoom.i][Laplas.curRoom.j]; i++)
									if (map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].type == 13)
									{
										Laplas.hitbox.x = map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].bordersHitbox.x;
										Laplas.hitbox.y = map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].bordersHitbox.y;
									}
							}
							break;

							

						case 13:
							if (HeroPhysicInRange({ Laplas.hitbox.x, Laplas.hitbox.y }, map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].bordersHitbox))
							{
								Laplas.lastLocalSwap = timeInGame;
								Laplas.curRoom.j++;
								for (int i = 0; i < map.bordersCount[Laplas.curRoom.i][Laplas.curRoom.j]; i++)
									if (map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].type == 12)
									{
										Laplas.hitbox.x = map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].bordersHitbox.x;
										Laplas.hitbox.y = map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].bordersHitbox.y;
									}
							}
							break;

						case 14:
							if (HeroPhysicInRange({ Laplas.hitbox.x, Laplas.hitbox.y }, map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].bordersHitbox))
							{
								Laplas.lastLocalSwap = timeInGame;
								Laplas.curRoom.i++;
								for (int i = 0; i < map.bordersCount[Laplas.curRoom.i][Laplas.curRoom.j]; i++)
									if (map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].type == 11)
									{
										Laplas.hitbox.x = map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].bordersHitbox.x;
										Laplas.hitbox.y = map.borders[Laplas.curRoom.i][Laplas.curRoom.j][i].bordersHitbox.y;
									}
							}
							break;
						}
					}

				//Статуя прокачки
				if (CheckSkillFigure(&Laplas, &map.itemsCount[Laplas.curRoom.i][Laplas.curRoom.j], map.items[Laplas.curRoom.i][Laplas.curRoom.j], &window, ren, win, &texture_skill, fontNovemBig, &audio))
					dopInitHero(&Laplas);

				//Сброс резиста от дамага
				if (Laplas.effect.lastDamage + Laplas.effect.afterAtackResist < timeInGame)
					Laplas.effect.underAtack = 0;

				//Активация ловушки
				TrapActivate(&map.trapsCount[Laplas.curRoom.i][Laplas.curRoom.j], map.traps[Laplas.curRoom.i][Laplas.curRoom.j], &Laplas, &timeInGame);

				#pragma endregion 

				#pragma region BATTLE
				HeroCommonAtack(&Laplas, &timeInGame, &map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j], map.enemys[Laplas.curRoom.i][Laplas.curRoom.j], timeInGame);

				HeroShootAtack(&Laplas, &timeInGame, &map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j], map.enemys[Laplas.curRoom.i][Laplas.curRoom.j]);

				HeroDashAtack(&Laplas, &timeInGame, &map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j], map.enemys[Laplas.curRoom.i][Laplas.curRoom.j]);

				//Удаление врагов с поле боя
				EnemyDeath(&map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j], map.enemys[Laplas.curRoom.i][Laplas.curRoom.j], &Laplas, map.items[Laplas.curRoom.i][Laplas.curRoom.j], &map.itemsCount[Laplas.curRoom.i][Laplas.curRoom.j]);

				//Атака ловушкой по ГГ
				TrapAtack(&map.trapsCount[Laplas.curRoom.i][Laplas.curRoom.j], map.traps[Laplas.curRoom.i][Laplas.curRoom.j], &Laplas, &timeInGame);

				//Дальняя атака врага
				for (int t = 0; t < map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j]; t++)
				{
					if (map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].type == 4 && !map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.alive && map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].effect.atackCD + map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.lastShoot < timeInGame)
					{
						map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.lastShoot = timeInGame;
						if (map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].hitbox.x < Laplas.hitbox.x)
						{
							map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.shootAtackCentere.x = map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].hitbox.x + map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].hitbox.w / 2;
							map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.shootAtackCentere.y = map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].hitbox.y + map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].hitbox.h / 2;
							map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.bulletSpeed = TRAPS_BULLET_SPEED;
							map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.alive = 1;
						}
						else
						{
							map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.shootAtackCentere.x = map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].hitbox.x - map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].hitbox.w / 2;
							map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.shootAtackCentere.y = map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].hitbox.y + map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].hitbox.h / 2;
							map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.bulletSpeed = -TRAPS_BULLET_SPEED;
							map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.alive = 1;
						}
					}

					else if (!map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.everShoot && map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].type == 6 && map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].animation_type == 6 && !map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.alive)
					{
						map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.lastShoot = timeInGame;
						
						map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.shootAtackCentere.x = map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].hitbox.x;
						map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.shootAtackCentere.y = map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].hitbox.y + map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].hitbox.h ;
						map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.bulletSpeed = TRAPS_BULLET_SPEED;
						map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.alive = 1;
						map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][t].shoot.everShoot = 1;
					}
				}

				//Движение снаряда врага с коллизией
				for (int j = 0; j < map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j]; j++)
				{
					if (map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][j].shoot.alive && map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][j].type != 6)
					{
						map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][j].shoot.shootAtackCentere.x += map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][j].shoot.bulletSpeed;

						if (!Laplas.effect.underAtack && CheckShootHitbox(&map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][j].shoot.shootAtackCentere, { Laplas.hitbox.x + Laplas.hitbox.w / 2,Laplas.hitbox.y + Laplas.hitbox.h / 2, Laplas.hitbox.w, Laplas.hitbox.h }))
						{
							map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][j].shoot.alive = 0;
							Laplas.effect.underAtack = 1;
							Laplas.effect.lastDamage = timeInGame;
							if (!Laplas.buffs.itemBallActive)
							{
								Laplas.status.HP -= map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][j].status.DMG;
							}
							else
								Laplas.buffs.itemBallActive = 0;

							if (Laplas.status.HP <= 0)
							{
								Laplas.status.alive = 0;
							}
						}

					}

					else if (map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][j].shoot.alive && map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][j].type == 6)
					{
						map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][j].shoot.shootAtackCentere.y += map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][j].shoot.bulletSpeed / 2;

						if (!Laplas.effect.underAtack && CheckShootHitbox(&map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][j].shoot.shootAtackCentere, { Laplas.hitbox.x + Laplas.hitbox.w / 2,Laplas.hitbox.y + Laplas.hitbox.h / 2, Laplas.hitbox.w, Laplas.hitbox.h }))
						{
							map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][j].shoot.alive = 0;
							Laplas.effect.underAtack = 1;
							Laplas.effect.lastDamage = timeInGame;
							if (!Laplas.buffs.itemBallActive)
							{
								Laplas.status.HP -= map.enemys[Laplas.curRoom.i][Laplas.curRoom.j][j].status.DMG;
							}
							else
								Laplas.buffs.itemBallActive = 0;

							if (Laplas.status.HP <= 0)
							{
								Laplas.status.alive = 0;
							}
						}
					}
				}




				#pragma endregion 

				#pragma region DRAW

				//Задний фон
				SDL_RenderCopy(ren, texture_backGround.texture, NULL, NULL);

				DrawItem(&Laplas, map.items[Laplas.curRoom.i][Laplas.curRoom.j], &window, ren, map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].i, map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].j, &map.itemsCount[Laplas.curRoom.i][Laplas.curRoom.j], fontNovemSmall);
		
				//Враги и стены
				DrawHitbox(map.bordersCount[Laplas.curRoom.i][Laplas.curRoom.j], map.borders[Laplas.curRoom.i][Laplas.curRoom.j], &Laplas, &window, &texture_cobbleStone, &texture_platform, &texture_trampline, &texture_tmp_platform, ren,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].i,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].j);
				DrawEnemys(&map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j], map.enemys[Laplas.curRoom.i][Laplas.curRoom.j], &Laplas, &window,  &texture_buff_DMG, ren,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].i,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].j); 
				DrawTraps(&map.trapsCount[Laplas.curRoom.i][Laplas.curRoom.j], map.traps[Laplas.curRoom.i][Laplas.curRoom.j], &Laplas, &window, ren,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].i,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].j);

				//Отрисовка таймера
				if (lastTime != timeInGame / 1000 % 60)
				{
					lastTime = timeInGame / 1000 % 60;
					sprintf_s(timer_text, "%02i:%02i", timeInGame / 60000, lastTime);
					SDL_DestroyTexture(texture_timer.texture);
					texture_timer = CreateTimerFromText(timer_text, fontNovemBig, { 220, 20, 60, 255 }, ren);
				}

				//Отрисовка кол-ва боеприпасов
				sprintf_s(amunition_text, "%03i", Laplas.status.ammunition);
				SDL_DestroyTexture(texture_ammunition.texture);
				texture_ammunition = CreateAmmunitionFromText(amunition_text, fontNovemSmall, { 255, 215, 0, 255 });

		
				//Таймер и боезапас
				DrawHeroBullet(&Laplas, &window, ren,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].i,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].j);
				DrawTrapsBullet(&Laplas, &window, &map.trapsCount[Laplas.curRoom.i][Laplas.curRoom.j], map.traps[Laplas.curRoom.i][Laplas.curRoom.j], &texture_trap_dart, ren,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].i,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].j);

				//ГГ
				DrawMainHero(&Laplas, window, ren,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].i,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].j);

				DrawItemsEffect(ren, &Laplas,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].i,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].j, &window);

				DrawFakeWalls(map.bordersCount[Laplas.curRoom.i][Laplas.curRoom.j], map.borders[Laplas.curRoom.i][Laplas.curRoom.j], &Laplas, &window, &texture_cobbleStone_fake, ren,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].i,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].j);

				////Эффект бафа
				DrawBuffsEffect(ren, &Laplas,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].i,  map.levelSize[Laplas.curRoom.i][Laplas.curRoom.j].j, &window);

				//отрисовка характеристик персонажа
				DrawAmmoBar(ammoBarTexture, &window, ren);
				DrawLifeBar(Laplas,hpBarTexture, hpBarEdgingTexture, &window, ren);
				DrawMoney(ren, &window, fontNovemSmall, Laplas);

				//отрисовка хп ближайшего противника
				if (map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j]>0)
				{
					DrawEnemyHP(Laplas, map.enemys[Laplas.curRoom.i][Laplas.curRoom.j], map.enemysCount[Laplas.curRoom.i][Laplas.curRoom.j], hpBarTexture, enemyHpBarEdgingTexture, &window, ren);
				}
				
				//Отрисовка пуль
				SDL_RenderCopy(ren, texture_timer.texture, NULL, &texture_timer.textureSize);
				SDL_RenderCopy(ren, texture_ammunition.texture, NULL, &texture_ammunition.textureSize);

				SDL_RenderPresent(ren);

				//Очистка
				SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
				SDL_RenderClear(ren);
				#pragma endregion 

				FPSControl();
			}
			SDL_RenderSetScale(ren, 1, 1);
			break;

		case CREDITS:
			CreditsMenu(&gameState, &window, ren, win);
			break;

		case SETTINGS:
			SettingsMenu(&gameState,&settings, &window, ren, win, audio);
			break;

		case PAUSE_MENU:
			StopMusic(&audio);
			PauseMenu(&gameState, &window, ren, win);
			break;

		case QUIT:
			flag = 0;
			break;
		}
		

	}

	//saveLaplas(&Laplas, "Saves\\1.txt", &settings);
	TTF_CloseFont(fontNovemBig);
	TTF_CloseFont(fontNovemSmall);

	SDL_DestroyTexture(texture_backGround.texture);
	SDL_DestroyTexture(texture_cobbleStone.texture);
	SDL_DestroyTexture(texture_cobbleStone_fake.texture);
	SDL_DestroyTexture(texture_timer.texture);
	SDL_DestroyTexture(texture_beaver_run.texture);
	SDL_DestroyTexture(texture_beaver_preatack.texture);
	SDL_DestroyTexture(texture_beaver_atack.texture);
	SDL_DestroyTexture(texture_platform.texture);
	SDL_DestroyTexture(texture_tmp_platform.texture);
	SDL_DestroyTexture(texture_trap_with_dart.texture);
	SDL_DestroyTexture(texture_pressure_plate.texture);
	SDL_DestroyTexture(texture_trap_dart.texture);
	SDL_DestroyTexture(texture_trap_spikes.texture);
	SDL_DestroyTexture(Laplas.animation.DMG_Buff.texture);
	SDL_DestroyTexture(Laplas.animation.dark.texture);
	SDL_DestroyTexture(texture_buff_DMG.texture);
	SDL_DestroyTexture(texture_buff_speed.texture);
	SDL_DestroyTexture(texture_buff_lucky.texture);
	SDL_DestroyTexture(texture_item_Rubber_Bullet.texture);
	SDL_DestroyTexture(texture_item_acid.texture);
	SDL_DestroyTexture(texture_item_Ball.texture);
	SDL_DestroyTexture(texture_barrel.texture);
	SDL_DestroyTexture(texture_acid_effect.texture);

	SDL_DestroyTexture(Laplas.animation.com.texture);
	SDL_DestroyTexture(Laplas.animation.bullet.texture);
	SDL_DestroyTexture(Laplas.animation.rubber_bullet.texture);
	SDL_DestroyTexture(Laplas.animation.punch.texture);
	SDL_DestroyTexture(Laplas.animation.run.texture);
	SDL_DestroyTexture(Laplas.animation.shoot.texture);
	SDL_DestroyTexture(texture_tower.texture);
	SDL_DestroyTexture(texture_tower_bullet.texture);
	
	SDL_DestroyTexture(texture_platform.texture);
	SDL_DestroyTexture(hpBarEdgingTexture.texture);
	SDL_DestroyTexture(enemyHpBarEdgingTexture.texture);
	SDL_DestroyTexture(hpBarTexture.texture);

	SDL_DestroyTexture(texture_skill.iconAmmo.texture);
	SDL_DestroyTexture(texture_skill.iconHealth.texture);
	SDL_DestroyTexture(texture_skill.iconLucky.texture);
	SDL_DestroyTexture(texture_skill.iconPotion.texture);
	SDL_DestroyTexture(texture_skill.iconStrength.texture);
	SDL_DestroyTexture(texture_skill.iconWeapon.texture);
	SDL_DestroyTexture(texture_skill.figure.texture);
	SDL_DestroyTexture(texture_skill.point.texture);

	SDL_DestroyTexture(texture_bat_with_anvil.texture);
	SDL_DestroyTexture(texture_bat_without_anvil.texture);
	SDL_DestroyTexture(texture_anvil_without_bat.texture);

	FreeMap(&map);

	DeInit(0, &win, &ren, &win_surface);
	
	return 0;
}
