#pragma once
#include "Global.h"


class Player {
public:
	//コンストラクタの宣言
	Player();
	Player(PlayerStruct player);
	//デストラクタの宣言
	~Player();

	//メンバ関数
	void Update(char* keys , char* oldkeys , int  map[MAP_HEIGHT][MAP_WIDTH] , Scroll& scroll);
	void Draw();
	double GetPlyaerTransformX();
	double GetPlyaerTransformY();

private:
	//メンバ変数
	PlayerStruct player;
};