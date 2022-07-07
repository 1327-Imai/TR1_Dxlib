#pragma once
#include "Global.h"

//ここはプレイヤーの処理で今回のテーマ外だから特に面白いことは書いてないよ

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
	Transform GetTransform();
	Transform GetOldTransform();

private:
	//メンバ変数
	PlayerStruct player;
};