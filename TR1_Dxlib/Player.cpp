#include "Player.h"
#include "Component.h"

//ここはプレイヤーの処理で今回のテーマ外だから特に面白いことは書いてないよ

//コンストラクタの定義
Player::Player() {
	//transform
	player.transform.pos.x = MAP_SIZE * 2;
	player.transform.pos.y = MAP_SIZE * (MAP_HEIGHT - 5);
	player.transform.r = 15;

	//speed
	player.speed.fixedValue = 3;

	//others
	player.aerialController.timer = 0;
	player.isJump = 0;
	player.aerialController.isInTheAir = 0;
}

Player::Player(PlayerStruct player) {
	this->player = player;
}

//デストラクタの定義
Player::~Player() {

}

//メンバ関数
//更新処理の関数
void Player::Update(char* keys , char* oldkeys , int  map[MAP_HEIGHT][MAP_WIDTH] , Scroll& scroll) {

	//以前の位置を取得
	for (int i = 4; 0 <= i; i--) {
		if (i == 0) {
			player.oldTransform[i] = player.transform;
		}
		else {
			player.oldTransform[i] = player.oldTransform[i - 1];
		}
	}

	//速度に0を代入
	player.speed.x = 0;
	player.speed.y = 0;
	player.speed.tmpX = 0;
	player.speed.tmpY = 0;

	//キー入力
	if (keys[KEY_INPUT_RIGHT] == 1 || keys[KEY_INPUT_D] == 1) {//右キーが押されたら

		player.speed.tmpX += player.speed.fixedValue;

	}
	if (keys[KEY_INPUT_LEFT] == 1 || keys[KEY_INPUT_A] == 1) {//左キーが押されたら

		player.speed.tmpX -= player.speed.fixedValue;

	}

	if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0) {
		if (player.isJump == false) {
			player.isJump = true;
			player.speed.initialspeedY = -5;
			player.aerialController.isInTheAir = true;
		}
		else if (player.isfloatJump == false) {
			player.isfloatJump = true;
			player.speed.initialspeedY = -5;
			player.aerialController.timer = 0;

		}
	}


	//地面にいるなら
	if (player.aerialController.isInTheAir == false) {
		//ジャンプフラグをfalseにする
		player.isJump = false;
		player.isfloatJump = false;
	}

	//重力の影響を受ける
	Component::AffectedByGravity(player.speed , player.aerialController);
	//マップチップとの当たり判定を行う
	Component::MapchipCollision(player.transform , player.speed , player.aerialController , map);

	player.transform.pos.x += player.speed.x;
	if (player.speed.x != 0) {
		if (player.speed.x < 0) {
			if (player.screen.x <= MAP_SIZE * 9) {
				if (0 < scroll.x) {//スクロール値が0より大きければ
					scroll.x += player.speed.x;//プレーヤーのスピード分スクロール値を減らす
				}
			}
		}
		else if (0 < player.speed.x) {
			if (MAP_SIZE * 11 <= player.screen.x) {
				if (scroll.x < scroll.MAX_X) {//スクロール値が0より大きければ
					scroll.x += player.speed.x;//プレーヤーのスピード分スクロール値を減らす
				}
			}
		}
	}

	player.transform.pos.y += player.speed.y;

	player.screen.x = player.transform.pos.x - scroll.x;
	player.screen.y = player.transform.pos.y - scroll.y;
}

//描画処理の関数
void Player::Draw() {
	DrawCircle(player.screen.x ,
			   player.screen.y ,
			   player.transform.r ,
			   GetColor(255 , 255 , 255) ,
			   true);
}

//アクセッサ
Transform Player::GetTransform() {
	return player.transform;
}

Transform Player::GetOldTransform() {
	return player.oldTransform[4];
}