#pragma once

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280
#define FPS 60
#define HERO_WIDHT 40
#define HERO_HEIGHT 40
#define X_MOVE 0
#define Y_MOVE 0
#define GAZE_DIRECTION 0
#define SPEED 10
#define GRAVITY 15
#define ACCELERATION 1
#define IMPULSE 0
#define ON_BORDER 0

struct mainPhysics
{
	float xMove, yMove;
	bool gazeDirection;
	float speed;
	float gravity;
	float acceleration;
	float impulse;
	bool onBorder;
};

struct mainHero
{
	float xPosition, yPosition;
	SDL_Rect hitbox;
	mainPhysics physic;
};
