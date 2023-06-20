#pragma once
#include <SDL.h>
#include <SDL_Image.h>
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

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
SDL_Surface* win_surface = NULL;
SDL_Event ev;
int levelWidth = 0, levelHeight = 0;

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

mainRenderer CreateTimerFromText(const char str[], TTF_Font* font, SDL_Color fg)
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

int main(int argc, char* argv[])
{
	int seed = 100;
	srand(seed);
	bool flag = 1;
	Init(&win, &ren, &win_surface);

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);

	#pragma region VARIABLE_INIT

	GameState gameState = MAIN_MENU;
	Settings settings = { 5, 1 };
	int tmp;

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
	mainRenderer texture_barrel;
	TTF_Font* fontNovemBig = NULL;
	TTF_Font* fontNovemSmall = NULL;

	
	mainRenderer hpBarTexture;
	mainRenderer hpBarEdgingTexture;
	mainRenderer ammoBarTexture;

	int bordersCount = NULL;
	int enemysCount = NULL;
	int trapsCount = NULL;
	int itemsCount = NULL;
	int lastTime = 0;
	int temp = 0;
	int timeInGame = clock();
	int check = 1;

	mainBorders* levelBorders = NULL;
	mainEnemys* levelEnemys = NULL;
	mainTraps* levelTraps = NULL;
	mainItems* levelItems = NULL;
	mainHero Laplas;
	
	static mainWindow window = { WINDOW_WIDTH ,WINDOW_HEIGHT };
	SDL_Point mouseClick = { NULL, NULL };
	
	char timer_text[10] = "00:00";
	char amunition_text[10] = "000";
	
	bool isRunning;

	#pragma endregion

	Laplas = InitHero();
	//loadLaplas(&Laplas, "Saves\\1\\main_hero.txt", &deltaTime);
	//dopLoadHero(&Laplas);

	#pragma region TEXTURES_LOAD

	GetTexture("Textures\\hero_comm.png", &Laplas.animation.com, 3, ren);
	GetTexture("Textures\\hero_run.png", &Laplas.animation.run, 8, ren);
	GetTexture("Textures\\hero_atack3.png", &Laplas.animation.punch, 5, ren);
	GetTexture("Textures\\hero_shoot.png", &Laplas.animation.shoot, 3, ren);
	GetTexture("Textures\\hero_bullet.png", &Laplas.animation.bullet, 1, ren);
	GetTexture("Textures\\hero_rubber_bullet.png", &Laplas.animation.rubber_bullet, 1, ren);
	Laplas.texture_rect.w = Laplas.texture_rect.h = Laplas.hitbox.h / 22. * 32;

	GetTexture("Textures\\bobr_run.png", &texture_beaver_run, 6, ren);
	GetTexture("Textures\\bobr_preatack.png", &texture_beaver_preatack, 1, ren);
	GetTexture("Textures\\bobr_atack.png", &texture_beaver_atack, 2, ren);

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

	GetTexture("Textures\\overheating.png", &texture_overheating, 1, ren);

	GetTexture("Textures\\dark.png", &texture_dark, 1, ren);

	GetTexture("Textures\\trampline.png", &texture_trampline, 1, ren);

	GetTexture("Textures\\buff_Rubber_Bullet.png", &texture_item_Rubber_Bullet, 12, ren);

	GetTexture("Textures\\buff_DMG.png", &texture_buff_DMG, 1, ren);

	GetTexture("Textures\\barrel.png", &texture_barrel, 1, ren);

	GetTexture("Textures\\life_bar.png", &hpBarTexture, 1, ren);
	GetTexture("Textures\\outside_life_bar.png", &hpBarEdgingTexture, 1, ren);
	GetTexture("Textures\\ammo_bar.png", &ammoBarTexture, 1, ren);
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

	levelBorders = LoadLevel(levelBorders, &bordersCount, &Laplas, "Levels\\SaveRoom1.txt", &levelWidth, &levelHeight);
	levelEnemys = LoadEnemys(levelEnemys, &enemysCount, "Enemys\\SaveRoomEnemys.txt");
	levelTraps = LoadTraps(levelTraps, &trapsCount, "Traps\\SaveRoomTraps.txt");
	levelItems = LoadItems(levelItems, &itemsCount, "Items\\Item.txt");

	InitEnemys(levelEnemys, &enemysCount, &texture_beaver_run, &texture_beaver_atack, &texture_beaver_preatack, &texture_krab);
	InitTraps(levelTraps, &trapsCount, &texture_trap_with_dart, &texture_pressure_plate, &texture_trap_spikes);
	InitItems(levelItems, &itemsCount, &texture_buff_DMG, &texture_item_Rubber_Bullet, &texture_barrel);

	//mainRoom mainMap[MAP_SIZE][MAP_SIZE];
	//for (int i = 0; i < MAP_SIZE; i++)
	//	for (int j = 0; j < MAP_SIZE; j++)
	//	{
	//		mainMap[i][j].type = 0;
	//		mainMap[i][j].left = 0;
	//		mainMap[i][j].right = 0;
	//		mainMap[i][j].top = 0;
	//		mainMap[i][j].down = 0;
	//	}

	//mainMap[MAP_SIZE / 2][0].type = -1;
	//mainMap[MAP_SIZE / 2][0].right = 1;
	//Laplas.curRoom.i = MAP_SIZE / 2;
	//Laplas.curRoom.j = MAP_SIZE / 2;

	//twoParam tmpCurRoom = Laplas.curRoom;
 //
	//tmp = rand() % (MAP_SIZE - 3) + 2;
	//for(int i = 1; i < tmp; i ++)
	//	mainMap[MAP_SIZE/2][i] = 

	while (flag)
	{
		switch (gameState)
		{
		case MAIN_MENU:
			MainMenu(&gameState, &window, ren, win);
			break;

		case IN_GAME:
			isRunning = true;
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
							break;

						case SDL_SCANCODE_A:
							Laplas.physic.xMoveL = -1;
							break;

						case SDL_SCANCODE_D:
							Laplas.physic.xMoveR = 1;
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
							break;

						case SDL_SCANCODE_S:
							Laplas.keys.pressed_S = 0;
							break;

						case SDL_SCANCODE_D:
							Laplas.physic.xMoveR = 0;
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
							}
						}
						break;
					}



				}
				#pragma endregion

				#pragma region PHYSIC_CHECK

				//��������� ���������
				HeroPhysicGetBase(&Laplas);
				EnemyPhysicGetBase(levelEnemys, &enemysCount);

				//�������� �� ��� X + �����
				if(Laplas.animationType != 2 && Laplas.animationType != 3)
					HeroPhysicXmovement(&Laplas);

				//������
				HeroPhysicJump(&Laplas);
				EnemyPhysicJump(levelEnemys, &enemysCount, &Laplas);

				//����������
				HeroPhysicGravity(&Laplas);
				EnemyPhysicGravity(levelEnemys, &enemysCount);

				//�������� ��������
				HeroPhysicHitboxOverlay(&bordersCount, &Laplas, levelBorders, &trapsCount, levelTraps, timeInGame);
				EnemyPhysicHitboxOverlay(&bordersCount, &enemysCount, levelEnemys, levelBorders, timeInGame, &Laplas);

				//�������� ���� �� ������� � ������
				TrapBulletHitboxInRange(levelTraps, &trapsCount, &bordersCount, levelBorders);
				HeroBulletHitboxInRange(&Laplas, &bordersCount, levelBorders);

				//����� �� ������� ����
				HeroPhysicOutworldCheck(&Laplas, levelBorders);
				EnemyPhysicOutworldCheck(&enemysCount, levelEnemys, levelBorders);
				HeroBulletOutworldCheck(&Laplas, levelBorders);
				TrapBulletOutworldCheck(levelTraps, &trapsCount, levelBorders);

				//������������ �������� ����������� ������
				EnemyTrigger(levelEnemys, &Laplas, &enemysCount);

				//�������� ������
				EnemysMovement(&enemysCount, levelEnemys, &Laplas);

				//�������� �����
				for (int i = 0; i < enemysCount; i++)
					if (levelEnemys[i].hitbox.x > Laplas.hitbox.x - levelEnemys[i].hitbox.w / 1.25 && levelEnemys[i].hitbox.x < Laplas.hitbox.x + levelEnemys[i].hitbox.w/ 1.25)
						levelEnemys[i].animation_type = 6;
					else
						levelEnemys[i].animation_type = 1;

				//������� �� ������ �������
				for(int i = 0; i < bordersCount;i++ )
					if (levelBorders[i].type == 3)
					{
						if (HeroPhysicInRange({ Laplas.hitbox.x, Laplas.hitbox.y }, levelBorders[i].bordersHitbox))
						{
							levelBorders = LoadLevel(levelBorders, &bordersCount, &Laplas, "Levels\\Borders1.txt", &levelWidth, &levelHeight);
							levelEnemys = LoadEnemys(levelEnemys, &enemysCount, "Enemys\\Enemy1.txt");
							InitEnemys(levelEnemys, &enemysCount, &texture_beaver_run, &texture_beaver_atack, &texture_beaver_preatack, &texture_krab);
							levelTraps = LoadTraps(levelTraps, &trapsCount, "Traps\\Trap1.txt");
							InitTraps(levelTraps, &trapsCount, &texture_trap_with_dart, &texture_pressure_plate, &texture_trap_spikes);
							levelItems = LoadItems(levelItems, &itemsCount, "Items\\Item.txt");
							InitItems(levelItems, &itemsCount, &texture_buff_DMG, &texture_item_Rubber_Bullet, &texture_barrel);
						}
					}

				#pragma endregion 

				#pragma region LOGIC_CHECK

				//������ ��������
				ItemEquip(&Laplas, levelItems);

				#pragma endregion 

				#pragma region BATTLE
				HeroCommonAtack(&Laplas, &timeInGame, &enemysCount, levelEnemys);

				HeroShootAtack(&Laplas, &timeInGame, &enemysCount, levelEnemys);

				HeroDashAtack(&Laplas, &timeInGame, &enemysCount, levelEnemys);

				//�������� ������ � ���� ���
				EnemyDeath(&enemysCount, levelEnemys);

				//��������� �������
				TrapActivate(&trapsCount, levelTraps, &Laplas, &timeInGame);
					
				//����� �������� �� ��
				TrapAtack(&trapsCount, levelTraps, &Laplas, &timeInGame);

				//����� ������� �� ������
				if (Laplas.effect.lastDamage + Laplas.effect.afterAtackResist < timeInGame)
					Laplas.effect.underAtack = 0;

				#pragma endregion 

				#pragma region DRAW

				//������ ���
				SDL_RenderCopy(ren, texture_backGround.texture, NULL, NULL);
		
				//����� � �����
				DrawHitbox(bordersCount, levelBorders, &Laplas, &window, &texture_cobbleStone, &texture_platform, &texture_trampline, &texture_tmp_platform, ren, levelWidth, levelHeight);
				DrawEnemys(&enemysCount, levelEnemys, &Laplas, &window,  &texture_buff_DMG, ren, levelWidth, levelHeight);
				DrawTraps(&trapsCount, levelTraps, &Laplas, &window, ren, levelWidth, levelHeight);

				//��������� �������
				if (lastTime != timeInGame / 1000 % 60)
				{
					lastTime = timeInGame / 1000 % 60;
					sprintf_s(timer_text, "%02i:%02i", timeInGame / 60000, lastTime);
					SDL_DestroyTexture(texture_timer.texture);
					texture_timer = CreateTimerFromText(timer_text, fontNovemBig, { 220, 20, 60, 255 });
				}

				//��������� ���-�� �����������
				sprintf_s(amunition_text, "%03i", Laplas.status.ammunition);
				SDL_DestroyTexture(texture_ammunition.texture);
				texture_ammunition = CreateAmmunitionFromText(amunition_text, fontNovemSmall, { 255, 215, 0, 255 });

		
				//������ � ��������
				DrawHeroBullet(&Laplas, &window, ren, levelWidth, levelHeight);
				DrawTrapsBullet(&Laplas, &window, &trapsCount, levelTraps, &texture_trap_dart, ren, levelWidth, levelHeight);

				DrawItem(&Laplas, levelItems, &window, ren, levelWidth, levelHeight, &itemsCount);

				//��
				DrawMainHero(&Laplas, window, ren, levelWidth, levelHeight);
				
				DrawFakeWalls(bordersCount, levelBorders, &Laplas, &window, &texture_cobbleStone_fake, ren, levelWidth, levelHeight);

				////������ ����
				//SDL_RenderCopy(ren, texture_dark.texture, NULL, NULL);

				DrawAmmoBar(ammoBarTexture, &window, ren);
				DrawLifeBar(Laplas,hpBarTexture, hpBarEdgingTexture, &window, ren);

				//��������� ����
				SDL_RenderCopy(ren, texture_timer.texture, NULL, &texture_timer.textureSize);
				SDL_RenderCopy(ren, texture_ammunition.texture, NULL, &texture_ammunition.textureSize);


				SDL_RenderPresent(ren);

				//�������
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
			SettingsMenu(&gameState,&settings, &window, ren, win);
			break;

		case PAUSE_MENU:
			PauseMenu(&gameState, &window, ren, win);
			break;

		case QUIT:
			flag = 0;
			break;
		}
		

	}

	//saveLaplas(&Laplas, "Saves\\1\\main_hero.txt", &deltaTime);

	free(levelBorders);
	free(levelEnemys);
	free(levelTraps);
	free(levelItems);
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
	SDL_DestroyTexture(texture_overheating.texture);
	SDL_DestroyTexture(texture_dark.texture);
	SDL_DestroyTexture(texture_buff_DMG.texture);
	SDL_DestroyTexture(texture_item_Rubber_Bullet.texture);
	SDL_DestroyTexture(texture_barrel.texture);


	SDL_DestroyTexture(Laplas.animation.com.texture);
	SDL_DestroyTexture(Laplas.animation.bullet.texture);
	SDL_DestroyTexture(Laplas.animation.rubber_bullet.texture);
	SDL_DestroyTexture(Laplas.animation.punch.texture);
	SDL_DestroyTexture(Laplas.animation.run.texture);
	SDL_DestroyTexture(Laplas.animation.shoot.texture);
	
	
	SDL_DestroyTexture(texture_platform.texture);
	SDL_DestroyTexture(hpBarEdgingTexture.texture);
	SDL_DestroyTexture(hpBarTexture.texture);
	SDL_DestroyTexture(ammoBarTexture.texture);

	DeInit(0, &win, &ren, &win_surface);
	
	return 0;
}
