#pragma once
#include "Global.h"

class Weapon {
public:
	//コンストラクタの宣言
	Weapon(int weaponType);

	//デストラクタの宣言
	~Weapon();

	//メンバ関数
	void Update(double x , double y , double angle , Scroll scroll , int map[MAP_HEIGHT][MAP_WIDTH]);

	void Draw();

	//セッター,ゲッター
	void SetIsShot(int isShot);

private:
	//メンバ変数

	Transform tipTransform;
	Transform rootTransform;
	Screen tipScreen;
	Screen rootScreen;
	Speed speed;
	double angle;
	int weaponType;
	int isShot;

};

