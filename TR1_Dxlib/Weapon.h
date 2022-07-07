#pragma once
#include "Global.h"

class Weapon {
public:
	//コンストラクタの宣言
	Weapon(int weaponType);

	//デストラクタの宣言
	~Weapon();

	//メンバ関数
	void Update(Transform transform , Scroll scroll , int map[MAP_HEIGHT][MAP_WIDTH]);

	void Draw();

	//アクセッサ
	void SetIsShot(int isShot);
	
	Speed GetSpeed();
	void SetSpeedFixedValue(float fixedValue);

	void SetAngle(float angle);

	float GetLength();

	void SetLength(float length);

private:
	//メンバ変数

	Transform tipTransform;
	Transform rootTransform;
	Screen tipScreen;
	Screen rootScreen;
	Speed speed;
	float angle;
	int weaponType;
	int isShot;

};

