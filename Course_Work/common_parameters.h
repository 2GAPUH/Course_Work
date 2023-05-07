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
#define SPEED 7
#define GRAVITY 10
#define ACCELERATION_Y 1
#define ACCELERATION_X 1
#define IMPULSE 0
#define ON_BORDER 0
#define EPS 1E-9
#define DASH_CD 700
#define ATACK_CD 700
#define CAMERA_SCALE_X 0
#define CAMERA_SCALE_Y 0
#define HERO_DAMAGE 40
#define HERO_HP 100
#define ALIVE 1
#define TIMER_SIZE 50
#define PRESSED_S 0

#pragma region BEAVER_STATS

#define BEAVER_HP 100
#define BEAVER_DMG 10
#define BEAVER_SPEED 5
#define BEAVER_ATACK_CD 1500

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