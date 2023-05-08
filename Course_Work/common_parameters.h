#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <math.h>
#include <stdio.h>

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280
#define FPS 60
#define HERO_WIDHT 80
#define HERO_HEIGHT 80
#define X_MOVE_L 0
#define X_MOVE_R 0
#define Y_MOVE 0
#define GAZE_DIRECTION 1
#define GRAVITY 10
#define ACCELERATION_Y 1
#define ACCELERATION_X 1
#define IMPULSE 0
#define ON_BORDER 0
#define EPS 1E-9
#define CAMERA_SCALE_X 0
#define CAMERA_SCALE_Y 0
#define ALIVE 1
#define TIMER_TEXT_SIZE 50
#define AMMUNITION_TEXT_SIZE 30
#define PRESSED_S 0
#define HERO_START_AMUNITION 30

#pragma region HERO_STATS

#define HERO_HP 100
#define HERO_DAMAGE 40
#define HERO_SHOOT_DAMAGE 50
#define HERO_DASH_CD 700
#define HERO_SPEED 7
#define HERO_BULLET_SPEED 21
#define HERO_ATACK_CD 500
#define HERO_SHOOT_CD 300 
#define HERO_BULLET_WIDTH 18 
#define HERO_BULLET_HIGHT 4 
#define HERO_AFTER_ATACK_PROTECTION 500 

#pragma endregion

#pragma region BEAVER_STATS

#define BEAVER_HP 100
#define BEAVER_DMG 10
#define BEAVER_SPEED 5
#define BEAVER_ATACK_CD 1500

#pragma endregion

#pragma region TRAPS_STATS

#define TRAP_DART_WIDTH 24 
#define TRAP_DART_HIGHT 14 
#define TRAP_DART_DMG 50 

#pragma endregion

struct mainWindow
{
	int w, h;
	float scaleX, scaleY;
};

typedef enum GameState {
	IN_GAME,
	SETTINGS,
	CREDITS,
	QUIT,
	MAIN_MENU,
	PAUSE_MENU
};

struct Settings
{
	int volume;
	int skin;
};

struct mainStatistic
{
	int DMG;
	int HP;
	bool alive;
	int ammunition;
	int Shoot_DMG;
};

struct mainPhysics
{
	float xMoveL, xMoveR, yMove;
	int gazeDirection;
	float speed;
	float gravity;
	float accelerationY;
	float accelerationX;
	float impulse;
	bool onBorder;
	bool pressed_S;
};

struct statusEffect
{
	int dashCD, timeDashCD;
	int atackCD, timeAtackCD;
	int shootCD, timeShootCD;
	SDL_Point camersScale;
	bool underAtack;
	int afterAtackResist;
	int lastDamage;
};

struct mainRenderer
{
	SDL_Texture* texture;
	SDL_Rect textureSize;
	SDL_Rect frame;
	int frameTime;
	int frameCount;
};

struct heroAnimation
{
	mainRenderer com;
	mainRenderer run;
	mainRenderer punch;
	mainRenderer shoot;
	mainRenderer bullet;
};

struct mainShoot
{
	bool alive;
	int bulletSpeed;
	SDL_Point shootAtackCentere;
};

struct mainBattle
{
	bool commonAtack;
	SDL_Point commonAtackCentere;
	bool shootAtack;
	mainShoot shoot[10];
};

struct mainHero
{
	SDL_Point position;
	SDL_Rect hitbox;
	mainPhysics physic;
	statusEffect effect;
	heroAnimation animation;
	mainBattle battle;
	mainStatistic status;
	int animationType;
};
/*
0 - стойка
1 - бег
2 - атака мечом
3 - атака пистолетом
4 - дэш
5 - нахождение в воздухе
*/


struct mainBorders
{
	int type;
	SDL_Rect bordersHitbox;
};

struct mainEnemys
{
	int type;
	SDL_Point position;
	SDL_Rect hitbox;
	mainPhysics physic;
	statusEffect effect;
	mainStatistic status;
	mainRenderer render;
};

struct mainTraps
{
	int type;
	SDL_Rect hitbox;
	mainShoot shoot;
	bool gazeDirection;
	mainRenderer render;
	bool triggered;
	int DMG;
};