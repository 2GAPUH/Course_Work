#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
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
#include "Audio.h"

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
SDL_Surface* win_surface = NULL;
SDL_Event ev;
Audio audio = {NULL,NULL, false, false};
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
	mainRenderer texture_shop;


	mainRenderer texture_item_Ball;
	mainRenderer texture_barrel;
	TTF_Font* fontNovemBig = NULL;
	TTF_Font* fontNovemSmall = NULL;

	
	mainRenderer hpBarTexture;
	mainRenderer hpBarEdgingTexture;
	mainRenderer enemyHpBarEdgingTexture;
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

	Laplas = InitHero();
	//loadLaplas(&Laplas, "Saves\\1\\main_hero.txt", &deltaTime);
	//dopLoadHero(&Laplas);

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

	InitEnemys(levelEnemys, &enemysCount, &texture_beaver_run, &texture_beaver_atack, &texture_beaver_preatack, &texture_krab, &texture_acid_effect, &texture_tower, &texture_tower_bullet, &texture_digit_idle, &texture_digit_atack, &texture_barrel);
	InitTraps(levelTraps, &trapsCount, &texture_trap_with_dart, &texture_pressure_plate, &texture_trap_spikes);
	InitItems(levelItems, &itemsCount, &texture_buff_DMG, &texture_item_Rubber_Bullet, &texture_barrel, &texture_item_Ball, 
		&texture_item_acid, &texture_buff_speed, &texture_buff_lucky, &texture_skill_figure, &texture_kebab, &texture_shop);

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
								PlaySound(shot_sound, 1, &audio, settings.soundsVolume, -1);
							}
						}
						break;
					}



				}
				#pragma endregion

				#pragma region PHYSIC_CHECK

				//Получение координат
				HeroPhysicGetBase(&Laplas);
				EnemyPhysicGetBase(levelEnemys, &enemysCount);

				//Движение по оси X + рывок
				if(Laplas.animationType != 2 && Laplas.animationType != 3)
					HeroPhysicXmovement(&Laplas);

				//Прыжок
				HeroPhysicJump(&Laplas);
				EnemyPhysicJump(levelEnemys, &enemysCount, &Laplas);

				//Гравитация
				HeroPhysicGravity(&Laplas);
				EnemyPhysicGravity(levelEnemys, &enemysCount);

				//Проверка коллизии
				HeroPhysicHitboxOverlay(&bordersCount, &Laplas, levelBorders, &trapsCount, levelTraps, timeInGame);
				EnemyPhysicHitboxOverlay(&bordersCount, &enemysCount, levelEnemys, levelBorders, timeInGame, &Laplas);

				//Проверка пуль на касание с стеной
				TrapBulletHitboxInRange(levelTraps, &trapsCount, &bordersCount, levelBorders);
				HeroBulletHitboxInRange(&Laplas, &bordersCount, levelBorders);
				EnemysBulletHitboxInRange(levelEnemys, &enemysCount, &bordersCount, levelBorders);

				//Выход за границы мира
				HeroPhysicOutworldCheck(&Laplas, levelBorders);
				EnemyPhysicOutworldCheck(&enemysCount, levelEnemys, levelBorders);
				HeroBulletOutworldCheck(&Laplas, levelBorders);
				TrapBulletOutworldCheck(levelTraps, &trapsCount, levelBorders);

				//Срабатывание триггера обнаружения врагом
				EnemyTrigger(levelEnemys, &Laplas, &enemysCount);

				//Движение врагов
				EnemysMovement(&enemysCount, levelEnemys, &Laplas);

				//Анимация атаки
				for (int i = 0; i < enemysCount; i++)
					if (levelEnemys[i].hitbox.x > Laplas.hitbox.x - levelEnemys[i].hitbox.w / 1.25 && levelEnemys[i].hitbox.x < Laplas.hitbox.x + levelEnemys[i].hitbox.w/ 1.25)
						levelEnemys[i].animation_type = 6;
					else
						levelEnemys[i].animation_type = 1;

				//Переход на другую локацию
				for(int i = 0; i < bordersCount;i++ )
					if (levelBorders[i].type == 3)
					{
						if (HeroPhysicInRange({ Laplas.hitbox.x, Laplas.hitbox.y }, levelBorders[i].bordersHitbox))
						{
							levelBorders = LoadLevel(levelBorders, &bordersCount, &Laplas, "Levels\\Borders3.txt", &levelWidth, &levelHeight);
							levelEnemys = LoadEnemys(levelEnemys, &enemysCount, "Enemys\\Enemy3.txt");
							InitEnemys(levelEnemys, &enemysCount, &texture_beaver_run, &texture_beaver_atack, &texture_beaver_preatack, &texture_krab, &texture_acid_effect, &texture_tower, &texture_tower_bullet, &texture_digit_idle, &texture_digit_atack, &texture_barrel);
							levelTraps = LoadTraps(levelTraps, &trapsCount, "Traps\\Trap3.txt");
							InitTraps(levelTraps, &trapsCount, &texture_trap_with_dart, &texture_pressure_plate, &texture_trap_spikes);
							levelItems = LoadItems(levelItems, &itemsCount, "Items\\Item3.txt");
							InitItems(levelItems, &itemsCount, &texture_buff_DMG, &texture_item_Rubber_Bullet, &texture_barrel, 
								&texture_item_Ball, &texture_item_acid, &texture_buff_speed, &texture_buff_lucky, &texture_skill_figure, &texture_kebab, &texture_shop);
						}
					}

				#pragma endregion 

				#pragma region LOGIC_CHECK

				//Подбор предмета
				ItemEquip(&Laplas, levelItems, &itemsCount,timeInGame);

				//Проверка таймера зелий
				BuffsStateCheck(&Laplas, timeInGame);

				//Проверка статуса врага
				for(int i = 0; i < enemysCount; i ++)
					if(levelEnemys[i].status.alive && levelEnemys[i].effect.poisoned && levelEnemys[i].status.HP > 2)
						if (levelEnemys[i].effect.poisonLastDamage + POISON_CD < timeInGame)
						{
							levelEnemys[i].effect.poisonLastDamage = timeInGame;
							levelEnemys[i].status.HP -= POISON_DMG;
						}


				CheckSkillFigure(&Laplas, &itemsCount, levelItems, &window, ren, win, &texture_skill);

				#pragma endregion 

				#pragma region BATTLE
				HeroCommonAtack(&Laplas, &timeInGame, &enemysCount, levelEnemys, timeInGame);

				HeroShootAtack(&Laplas, &timeInGame, &enemysCount, levelEnemys);

				HeroDashAtack(&Laplas, &timeInGame, &enemysCount, levelEnemys);

				//Удаление врагов с поле боя
				EnemyDeath(&enemysCount, levelEnemys, &Laplas, levelItems, &itemsCount);

				//Активация ловушки
				TrapActivate(&trapsCount, levelTraps, &Laplas, &timeInGame);
					
				//Атака ловушкой по ГГ
				TrapAtack(&trapsCount, levelTraps, &Laplas, &timeInGame);


				for (int t = 0; t < enemysCount; t++)
					if (levelEnemys[t].type == 4 && !levelEnemys[t].shoot.alive && levelEnemys[t].effect.atackCD + levelEnemys[t].shoot.lastShoot < timeInGame)
					{
						levelEnemys[t].shoot.lastShoot = timeInGame;
						if (levelEnemys[t].hitbox.x < Laplas.hitbox.x)
						{
							levelEnemys[t].shoot.shootAtackCentere.x = levelEnemys[t].hitbox.x + levelEnemys[t].hitbox.w / 2;
							levelEnemys[t].shoot.shootAtackCentere.y = levelEnemys[t].hitbox.y + levelEnemys[t].hitbox.h / 2;
							levelEnemys[t].shoot.bulletSpeed = TRAPS_BULLET_SPEED;
							levelEnemys[t].shoot.alive = 1;
						}
						else
						{
							levelEnemys[t].shoot.shootAtackCentere.x = levelEnemys[t].hitbox.x - levelEnemys[t].hitbox.w / 2;
							levelEnemys[t].shoot.shootAtackCentere.y = levelEnemys[t].hitbox.y + levelEnemys[t].hitbox.h / 2;
							levelEnemys[t].shoot.bulletSpeed = -TRAPS_BULLET_SPEED;
							levelEnemys[t].shoot.alive = 1;
						}
					}


				for (int j = 0; j < enemysCount; j++)
					if (levelEnemys[j].shoot.alive)
					{
						levelEnemys[j].shoot.shootAtackCentere.x += levelEnemys[j].shoot.bulletSpeed;

						if (!Laplas.effect.underAtack && CheckShootHitbox(&levelEnemys[j].shoot.shootAtackCentere, &Laplas.hitbox))
						{
							levelEnemys[j].shoot.alive = 0;
							Laplas.effect.underAtack = 1;
							Laplas.effect.lastDamage = timeInGame;
							if (!Laplas.buffs.itemBallActive)
							{
								Laplas.status.HP -= levelEnemys[j].status.DMG;
							}
							else
								Laplas.buffs.itemBallActive = 0;
									  
							if (Laplas.status.HP <= 0)
							{		  
								Laplas.status.alive = 0;
							}
						}

					}


				//Сброс резиста от дамага
				if (Laplas.effect.lastDamage + Laplas.effect.afterAtackResist < timeInGame)
					Laplas.effect.underAtack = 0;

				#pragma endregion 

				#pragma region DRAW

				//Задний фон
				SDL_RenderCopy(ren, texture_backGround.texture, NULL, NULL);

				DrawItem(&Laplas, levelItems, &window, ren, levelWidth, levelHeight, &itemsCount, fontNovemSmall);
		
				//Враги и стены
				DrawHitbox(bordersCount, levelBorders, &Laplas, &window, &texture_cobbleStone, &texture_platform, &texture_trampline, &texture_tmp_platform, ren, levelWidth, levelHeight);
				DrawEnemys(&enemysCount, levelEnemys, &Laplas, &window,  &texture_buff_DMG, ren, levelWidth, levelHeight);
				DrawTraps(&trapsCount, levelTraps, &Laplas, &window, ren, levelWidth, levelHeight);

				//Отрисовка таймера
				if (lastTime != timeInGame / 1000 % 60)
				{
					lastTime = timeInGame / 1000 % 60;
					sprintf_s(timer_text, "%02i:%02i", timeInGame / 60000, lastTime);
					SDL_DestroyTexture(texture_timer.texture);
					texture_timer = CreateTimerFromText(timer_text, fontNovemBig, { 220, 20, 60, 255 });
				}

				//Отрисовка кол-ва боеприпасов
				sprintf_s(amunition_text, "%03i", Laplas.status.ammunition);
				SDL_DestroyTexture(texture_ammunition.texture);
				texture_ammunition = CreateAmmunitionFromText(amunition_text, fontNovemSmall, { 255, 215, 0, 255 });

		
				//Таймер и боезапас
				DrawHeroBullet(&Laplas, &window, ren, levelWidth, levelHeight);
				DrawTrapsBullet(&Laplas, &window, &trapsCount, levelTraps, &texture_trap_dart, ren, levelWidth, levelHeight);

				//ГГ
				DrawMainHero(&Laplas, window, ren, levelWidth, levelHeight);
				if (Laplas.animation.punch.frame.x / (Laplas.animation.punch.frameCount - 1) == 1 || Laplas.animation.shoot.frame.x / (Laplas.animation.shoot.frameCount - 1) == 1)
				{
					StopSound(&audio, 1);
				}

				DrawItemsEffect(ren, &Laplas, levelWidth, levelHeight, &window);

				DrawFakeWalls(bordersCount, levelBorders, &Laplas, &window, &texture_cobbleStone_fake, ren, levelWidth, levelHeight);

				////Эффект бафа
				DrawBuffsEffect(ren, &Laplas, levelWidth, levelHeight, &window);

				//отрисовка характеристик персонажа
				DrawAmmoBar(ammoBarTexture, &window, ren);
				DrawLifeBar(Laplas,hpBarTexture, hpBarEdgingTexture, &window, ren);
				DrawMoney(ren, &window, fontNovemSmall, Laplas);

				//отрисовка хп ближайшего противника
				if (enemysCount>0)
				{
					DrawEnemyHP(Laplas, levelEnemys, enemysCount, hpBarTexture, enemyHpBarEdgingTexture, &window, ren);
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


	DeInit(0, &win, &ren, &win_surface);
	
	return 0;
}
