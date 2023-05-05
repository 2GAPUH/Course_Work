#pragma once

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280
#define FPS 60
#define HERO_WIDHT 105
#define HERO_HEIGHT 60
#define X_MOVE_L 0
#define X_MOVE_R 0
#define Y_MOVE 0
#define GAZE_DIRECTION 1
#define SPEED 5
#define GRAVITY 10
#define ACCELERATION_Y 1
#define ACCELERATION_X 1
#define IMPULSE 0
#define ON_BORDER 0
#define EPS 1E-9
#define DASH_CD 700
#define ATACK_CD 1000
#define CAMERA_SCALE_X 0
#define CAMERA_SCALE_Y 0
#define HERO_DAMAGE 40
#define HERO_HP 100
#define ALIVE 1

struct mainWindow
{
	int w, h;
	float scaleX, scaleY;
};

struct mainStatistic
{
	int DMG;
	int HP;
	bool alive;
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
};

struct statusEffect
{
	int dashCD, timeDashCD;
	int atackCD, timeAtackCD;
	SDL_Point camersScale;
	bool underAtack;
};

struct mainRenderer
{
	SDL_Texture* texture;
	SDL_Rect textureSize;
	SDL_Rect frame;
	int frameTime;
};

struct mainBattle
{
	bool commonAtack;
	SDL_Point commonAtackCentere;
};

struct mainHero
{
	SDL_Point position;
	SDL_Rect hitbox;
	mainPhysics physic;
	statusEffect effect;
	mainRenderer render;
	mainBattle battle;
	mainStatistic status;
};

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
};