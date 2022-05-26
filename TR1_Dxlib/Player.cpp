#include "Player.h"

//コンストラクタの定義
Player::Player() {
	//transform
	player.transform.x = MAP_SIZE * 2;
	player.transform.y = MAP_SIZE * (MAP_HEIGHT - 5);
	player.transform.r = 15;

	//speed
	player.speed.fixedValue = 3;

	//others
	player.AirTimer = 0;
	player.isJump = 0;
	player.isInTheAir = 0;
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
	player.speed.x = 0;
	player.speed.y = 0;
	player.speed.tmpX = player.speed.fixedValue;
	player.speed.tmpY = 0;

	//プレイヤーの本来の四隅の縦方向の座標を取得する
	player.mapColider.leftTopY = ((int)player.transform.y - (int)player.transform.r) / MAP_SIZE;
	player.mapColider.leftBottomY = ((int)player.transform.y + (int)player.transform.r - 1) / MAP_SIZE;
	player.mapColider.rightTopY = ((int)player.transform.y - (int)player.transform.r) / MAP_SIZE;
	player.mapColider.rightBottomY = ((int)player.transform.y + (int)player.transform.r - 1) / MAP_SIZE;

	//プレイヤーの本来の四隅の横方向の座標を取得する
	player.mapColider.rightTopX = ((int)player.transform.x + (int)player.transform.r - 1) / MAP_SIZE;
	player.mapColider.rightBottomX = ((int)player.transform.x + (int)player.transform.r - 1) / MAP_SIZE;
	player.mapColider.leftTopX = ((int)player.transform.x - (int)player.transform.r) / MAP_SIZE;
	player.mapColider.leftBottomX = ((int)player.transform.x - (int)player.transform.r) / MAP_SIZE;

	if (player.isInTheAir == false) {

		if (player.transform.x + player.transform.r < MAP_WIDTH * MAP_SIZE) {//プレイヤーが右端より左にいるとき
			if (keys[KEY_INPUT_RIGHT] == 1 || keys[KEY_INPUT_D] == 1) {//右キーが押されたら
				//現在のプレイヤーの右上、右下の頂点に一回分右方向に進んだ場合の座標を取得
				player.mapColider.rightTopX = (player.transform.x + player.speed.tmpX + player.transform.r - 1) / MAP_SIZE;
				player.mapColider.rightBottomX = (player.transform.x + player.speed.tmpX + player.transform.r - 1) / MAP_SIZE;
				//取得した座標の位置のマップチップがNONEであればプレイヤーの横方向のスピードが右に行くように値を入れる
				if (map[player.mapColider.rightTopY][player.mapColider.rightTopX] == NONE && map[player.mapColider.rightBottomY][player.mapColider.rightBottomX] == NONE) {
					player.speed.x += player.speed.fixedValue;
					if (MAP_SIZE * 11 <= player.screen.x) {//プレイヤーが15マス目以降にいるとき
						if (scroll.x < scroll.MAX_X) {//スクロール値が最大値より小さければ
							scroll.x += player.speed.fixedValue;//プレーヤーのスピード分スクロール値を増やす
						}
					}
				}
			}
		}

		if (player.transform.x + player.transform.r < MAP_WIDTH * MAP_SIZE) {//プレイヤーが右端より左にいるとき
			if (keys[KEY_INPUT_LEFT] == 1 || keys[KEY_INPUT_A] == 1) {//右キーが押されたら
				//現在のプレイヤーの左上、左下の頂点に一回分左方向に進んだ場合の座標を取得
				player.mapColider.leftTopX = (player.transform.x - player.speed.tmpX - player.transform.r) / MAP_SIZE;
				player.mapColider.leftBottomX = (player.transform.x - player.speed.tmpX - player.transform.r) / MAP_SIZE;
				//取得した座標の位置のマップチップがNONEであればプレイヤーの横方向のスピードが左に行くように値を入れる
				if (map[player.mapColider.leftTopY][player.mapColider.leftTopX] == NONE && map[player.mapColider.leftBottomY][player.mapColider.leftBottomX] == NONE) {
					player.speed.x -= player.speed.fixedValue;
					if (player.screen.x <= MAP_SIZE * 9) {//プレイヤーが5マス目以前にいるとき
						if (0 < scroll.x) {//スクロール値が0より大きければ
							scroll.x -= player.speed.fixedValue;//プレーヤーのスピード分スクロール値を減らす
						}
					}
				}
			}
		}

		//実際にプレイヤーを進める
		player.transform.x += player.speed.x;

		//プレイヤーの本来の四隅の横方向の座標を取得する
		player.mapColider.rightTopX = (player.transform.x + player.transform.r - 1) / MAP_SIZE;
		player.mapColider.rightBottomX = (player.transform.x + player.transform.r - 1) / MAP_SIZE;
		player.mapColider.leftTopX = (player.transform.x - player.transform.r) / MAP_SIZE;
		player.mapColider.leftBottomX = (player.transform.x - player.transform.r) / MAP_SIZE;

		if (player.isJump == false) {
			if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0) {
				player.isJump = true;
				player.speed.initialspeedY = -5;
				player.isInTheAir = true;
			}
		}

		if (map[player.mapColider.rightBottomY + 1][player.mapColider.rightBottomX] == 0 &&
			map[player.mapColider.leftBottomY + 1][player.mapColider.leftBottomX] == 0) {
			player.isInTheAir = true;
			player.speed.initialspeedY = 0;
		}
	}
	else if (player.isInTheAir == true) {

		if (player.isDoubleJump == false) {
			if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0) {
				player.isDoubleJump = true;
				player.speed.initialspeedY = -5;
				player.AirTimer = 0;
			}
		}

		player.speed.tmpY = player.speed.initialspeedY + (G / 50) * player.AirTimer;

		//現在のプレイヤーの四隅の縦方向の座標に一回分進んだ場合の座標を取得
		player.mapColider.leftTopY = ((int)player.transform.y + player.speed.tmpY - (int)player.transform.r) / MAP_SIZE;
		player.mapColider.rightTopY = ((int)player.transform.y + player.speed.tmpY - (int)player.transform.r) / MAP_SIZE;
		player.mapColider.leftBottomY = ((int)player.transform.y + player.speed.tmpY + (int)player.transform.r - 1) / MAP_SIZE;
		player.mapColider.rightBottomY = ((int)player.transform.y + player.speed.tmpY + (int)player.transform.r - 1) / MAP_SIZE;

		if (player.speed.tmpY < 0) {
			if (map[player.mapColider.rightTopY][player.mapColider.rightTopX] == 0 &&
				map[player.mapColider.leftTopY][player.mapColider.leftTopX] == 0) {
				player.speed.y = player.speed.tmpY;
			}
			else {
				player.AirTimer = 0;
				player.speed.initialspeedY = 0;
			}
		}

		else if (0 <= player.speed.tmpY) {
			if (map[player.mapColider.rightBottomY][player.mapColider.rightBottomX] == 0 &&
				map[player.mapColider.leftBottomY][player.mapColider.leftBottomX] == 0) {
				player.speed.y = player.speed.tmpY;
			}
			else {
				player.transform.y = player.mapColider.rightBottomY * MAP_SIZE - 15;
				player.isInTheAir = false;
				player.isJump = 0;
				player.isDoubleJump = 0;
				player.AirTimer = 0;
			}
		}

		//実際にプレイヤーを進める
		player.transform.y += player.speed.y;

		//プレイヤーの本来の四隅の縦方向の座標を取得する
		player.mapColider.leftTopY = ((int)player.transform.y - (int)player.transform.r) / MAP_SIZE;
		player.mapColider.leftBottomY = ((int)player.transform.y + (int)player.transform.r - 1) / MAP_SIZE;
		player.mapColider.rightTopY = ((int)player.transform.y - (int)player.transform.r) / MAP_SIZE;
		player.mapColider.rightBottomY = ((int)player.transform.y + (int)player.transform.r - 1) / MAP_SIZE;

		if (player.transform.x + player.transform.r < MAP_WIDTH * MAP_SIZE) {//プレイヤーが右端より左にいるとき
			if (keys[KEY_INPUT_RIGHT] == 1 || keys[KEY_INPUT_D] == 1) {//右キーが押されたら
				//現在のプレイヤーの右上、右下の頂点に一回分右方向に進んだ場合の座標を取得
				player.mapColider.rightTopX = (player.transform.x + player.speed.tmpX + player.transform.r - 1) / MAP_SIZE;
				player.mapColider.rightBottomX = (player.transform.x + player.speed.tmpX + player.transform.r - 1) / MAP_SIZE;
				//取得した座標の位置のマップチップがNONEであればプレイヤーの横方向のスピードが右に行くように値を入れる
				if (map[player.mapColider.rightTopY][player.mapColider.rightTopX] == NONE && map[player.mapColider.rightBottomY][player.mapColider.rightBottomX] == NONE) {
					player.speed.x += player.speed.fixedValue;
					if (MAP_SIZE * 11 <= player.screen.x) {//プレイヤーが15マス目以降にいるとき
						if (scroll.x < scroll.MAX_X) {//スクロール値が最大値より小さければ
							scroll.x += player.speed.fixedValue;//プレーヤーのスピード分スクロール値を増やす
						}
					}
				}
			}
		}

		if (player.transform.x + player.transform.r < MAP_WIDTH * MAP_SIZE) {//プレイヤーが右端より左にいるとき
			if (keys[KEY_INPUT_LEFT] == 1 || keys[KEY_INPUT_A] == 1) {//右キーが押されたら
				//現在のプレイヤーの左上、左下の頂点に一回分左方向に進んだ場合の座標を取得
				player.mapColider.leftTopX = (player.transform.x - player.speed.tmpX - player.transform.r) / MAP_SIZE;
				player.mapColider.leftBottomX = (player.transform.x - player.speed.tmpX - player.transform.r) / MAP_SIZE;
				//取得した座標の位置のマップチップがNONEであればプレイヤーの横方向のスピードが左に行くように値を入れる
				if (map[player.mapColider.leftTopY][player.mapColider.leftTopX] == NONE && map[player.mapColider.leftBottomY][player.mapColider.leftBottomX] == NONE) {
					player.speed.x -= player.speed.fixedValue;
					if (player.screen.x <= MAP_SIZE * 9) {//プレイヤーが5マス目以前にいるとき
						if (0 < scroll.x) {//スクロール値が0より大きければ
							scroll.x -= player.speed.fixedValue;//プレーヤーのスピード分スクロール値を減らす
						}
					}
				}
			}
		}

		//実際にプレイヤーを進める
		player.transform.x += player.speed.x;

		//プレイヤーの本来の四隅の横方向の座標を取得する
		player.mapColider.rightTopX = ((int)player.transform.x + (int)player.transform.r - 1) / MAP_SIZE;
		player.mapColider.rightBottomX = ((int)player.transform.x + (int)player.transform.r - 1) / MAP_SIZE;
		player.mapColider.leftTopX = ((int)player.transform.x - (int)player.transform.r) / MAP_SIZE;
		player.mapColider.leftBottomX = ((int)player.transform.x - (int)player.transform.r) / MAP_SIZE;

		player.AirTimer++;
	}

	player.screen.x = player.transform.x - scroll.x;
	player.screen.y = player.transform.y - scroll.y;
}

//描画処理の関数
void Player::Draw() {
	DrawCircle(player.screen.x ,
			   player.screen.y ,
			   player.transform.r ,
			   GetColor(255 , 255 , 255) ,
			   true);
}

//ゲッター
double Player::GetPlyaerTransformX() {
	return player.transform.x;
}

double Player::GetPlyaerTransformY() {
	return player.transform.y;
}