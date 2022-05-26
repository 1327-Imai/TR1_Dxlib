#pragma once
#include "Global.h"
#include "Player.h"
#include "Weapon.h"

class Enemy {

public:
	//�R���X�g���N�^�̐錾
	Enemy(int enemyType);
	Enemy(EnemyStruct enemy);

	//�f�X�g���N�^�̐錾
	~Enemy();

	//�����o�֐�
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
	//�����o�ϐ�
	EnemyStruct enemy;
	Weapon* weapon;

};

