#pragma once
#include "Global.h"
#include "Player.h"
#include "Weapon.h"

class Enemy {

public:
	//コンストラクタの宣言
	Enemy(int enemyType);
	Enemy(EnemyStruct enemy);

	//デストラクタの宣言
	~Enemy();

	//メンバ関数
	void Update(Player player , int  map[MAP_HEIGHT][MAP_WIDTH] , Scroll& scroll);
	void Draw();

private:
	void MoveOfSlime(int  map[MAP_HEIGHT][MAP_WIDTH]);
	void MoveOfFencer(int  map[MAP_HEIGHT][MAP_WIDTH]);
	void MoveOfArcher(int  map[MAP_HEIGHT][MAP_WIDTH] , Player player);

	void AttackOfSlime(int  map[MAP_HEIGHT][MAP_WIDTH]);
	void AttackOfFencer(int  map[MAP_HEIGHT][MAP_WIDTH]);
	void AttackOfArcher(int  map[MAP_HEIGHT][MAP_WIDTH] , Player player);

	void DrawOfSlime();
	void DrawOfFencer();
	void DrawOfArcher();

private:
	//メンバ変数
	EnemyStruct enemy;
	Weapon* weapon;

};

