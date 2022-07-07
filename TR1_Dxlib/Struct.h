#pragma once
#include "Vector2.h"

typedef struct Transform {

	Vector2 pos;
	float r;

}Transform;

typedef struct Screen {

	float x;
	float y;

}Screen;

typedef struct Speed {

	float fixedValue;
	float x;
	float y;
	float tmpX;
	float tmpY;
	float initialspeedY;

}Speed;

typedef struct MapCollider {

	int rightTopX;
	int rightTopY;
	int rightBottomX;
	int rightBottomY;
	int leftTopX;
	int leftTopY;
	int leftBottomX;
	int leftBottomY;

}MapCollider;

typedef struct Scroll {

	float x;
	float y;
	float MAX_X;
	float MAX_Y;

}Scrool;

typedef struct AerialController{

	int isInTheAir;
	int timer;

}AerialController;

typedef struct PlayerStruct {

	Transform transform;
	Transform oldTransform[5];
	Screen screen;
	Speed speed;
	AerialController aerialController;
	int isJump;
	int isfloatJump;

}PlayerStruct;

typedef struct Eye {
	float x;
	float y;
	float angle;
	float fov;
	float range;
	int isVisuable;
}Eye;

typedef struct Attack {
	int isAttack;
	int coolTime;
	int isShot;
	int weaponType;
	float weaponAngle;
	float range;
	float amplitude;
	float angle;
	int motionTimer;
	int motionPhase;
}Attack;

typedef struct Target {

	float x;
	float y;

}Target;

typedef struct EnemyStruct {

	Transform transform;
	Screen screen;
	Speed speed;
	Attack attack;
	AerialController aerialController;
	Target target;
	Eye eye;
	int type;
	int isFacingRight;
	float distanceToPlayer;

}EnemyStruct;