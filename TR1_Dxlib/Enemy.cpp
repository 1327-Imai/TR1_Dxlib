#include "Enemy.h"

//�R���X�g���N�^�̒�`
Enemy::Enemy(int enemyType) {

	enemy.type = enemyType;

	if (enemy.type == SLIME) {
		//transform
		enemy.transform.x = MAP_SIZE * 10;
		enemy.transform.y = MAP_SIZE * ((double)MAP_HEIGHT - 3.5);
		enemy.transform.r = 15;

		//speed
		enemy.speed.fixedValue = 1;

		//range
		enemy.sensingRenge = 10 * MAP_SIZE;
		enemy.attackRenge = 6 * MAP_SIZE;

		//weapon
		enemy.weaponType = NOWEAPON;

	}

	else if (enemy.type == FENCER) {
		//transform
		enemy.transform.x = MAP_SIZE * 20;
		enemy.transform.y = MAP_SIZE * ((double)MAP_HEIGHT - 3.5);
		enemy.transform.r = 15;

		//speed
		enemy.speed.fixedValue = 2;

		//range
		enemy.sensingRenge = 10 * MAP_SIZE;
		enemy.attackRenge = 5 * MAP_SIZE;

		//angle
		enemy.angle = PI * 5 / 6;

		//weapon
		enemy.weaponType = SROWD;
	}

	else if (enemy.type == ARCHER) {
		//transform
		enemy.transform.x = MAP_SIZE * 30;
		enemy.transform.y = MAP_SIZE * ((double)MAP_HEIGHT - 3.5);
		enemy.transform.r = 15;

		//speed
		enemy.speed.fixedValue = 2;

		//range
		enemy.sensingRenge = 15 * MAP_SIZE;
		enemy.attackRenge = 9 * MAP_SIZE;

		//angle
		enemy.angle = PI;

		//weapon
		enemy.weaponType = ARROW;
		enemy.isShotArrow = false;
	}

	//weapon
	weapon = new Weapon(enemy.weaponType);

	//others
	enemy.AirTimer = 0;
	enemy.isInTheAir = 0;
	enemy.isFacingRight = false;
	enemy.attackCT = 0;
	enemy.motionTimer = 0;
	enemy.motionPhase = 0;
}

Enemy::Enemy(EnemyStruct enemy) {
	this->enemy = enemy;

	weapon = new Weapon(enemy.weaponType);
}

//�f�X�g���N�^�̒�`
Enemy::~Enemy() {
	delete weapon;
}

//�����o�֐�
void Enemy::Update(Player player , int  map[MAP_HEIGHT][MAP_WIDTH] , Scroll& scroll) {

	enemy.speed.x = 0;
	enemy.speed.y = 0;
	enemy.speed.tmpX = enemy.speed.fixedValue;
	enemy.speed.tmpY = 0;

	//�G�̖{���̎l���̏c�����̍��W���擾����
	enemy.mapColider.leftTopY = ((int)enemy.transform.y - (int)enemy.transform.r) / MAP_SIZE;
	enemy.mapColider.leftBottomY = ((int)enemy.transform.y + (int)enemy.transform.r - 1) / MAP_SIZE;
	enemy.mapColider.rightTopY = ((int)enemy.transform.y - (int)enemy.transform.r) / MAP_SIZE;
	enemy.mapColider.rightBottomY = ((int)enemy.transform.y + (int)enemy.transform.r - 1) / MAP_SIZE;

	//�G�̖{���̎l���̉������̍��W���擾����
	enemy.mapColider.rightTopX = ((int)enemy.transform.x + (int)enemy.transform.r - 1) / MAP_SIZE;
	enemy.mapColider.rightBottomX = ((int)enemy.transform.x + (int)enemy.transform.r - 1) / MAP_SIZE;
	enemy.mapColider.leftTopX = ((int)enemy.transform.x - (int)enemy.transform.r) / MAP_SIZE;
	enemy.mapColider.leftBottomX = ((int)enemy.transform.x - (int)enemy.transform.r) / MAP_SIZE;

	if (enemy.isInTheAir == false) {

		if (map[enemy.mapColider.rightBottomY + 1][enemy.mapColider.rightBottomX] == 0 &&
			map[enemy.mapColider.leftBottomY + 1][enemy.mapColider.leftBottomX] == 0) {
			enemy.isInTheAir = true;
			enemy.speed.initialspeedY = 0;
		}

	}

	enemy.distanceToPlayer = sqrt(
		pow(player.GetPlyaerTransformX() - enemy.transform.x , 2) +
		pow(player.GetPlyaerTransformY() - enemy.transform.y , 2)
	);

	if (enemy.distanceToPlayer <= enemy.attackRenge) {
		if (enemy.attackCT <= 0) {
			enemy.isAttack = true;
		}
	}

	if (enemy.isAttack == true) {
		if (enemy.type == SLIME) {
			AttackOfSlime(map);
		}

		else if (enemy.type == FENCER) {
			AttackOfFencer(map);
		}

		else if (enemy.type == ARCHER) {
			AttackOfArcher(map , player);
		}
	}

	else {
		if (enemy.distanceToPlayer <= enemy.sensingRenge) {

			if (enemy.transform.x < player.GetPlyaerTransformX()) {
				enemy.isFacingRight = true;
			}
			else {
				enemy.isFacingRight = false;
			}

			if (enemy.type == SLIME) {
				MoveOfSlime(map);
			}

			else if (enemy.type == FENCER) {
				MoveOfFencer(map);
			}

			else if (enemy.type == ARCHER) {
				MoveOfArcher(map , player);
			}
		}
	}

	if (enemy.isInTheAir == true) {
		if (enemy.isAttack == true) {
			if (enemy.type != SLIME) {
				enemy.speed.tmpY = enemy.speed.initialspeedY + (G / 50) * enemy.AirTimer;

				//���݂̃v���C���[�̎l���̏c�����̍��W�Ɉ�񕪐i�񂾏ꍇ�̍��W���擾
				enemy.mapColider.leftTopY = ((int)enemy.transform.y + enemy.speed.tmpY - (int)enemy.transform.r) / MAP_SIZE;
				enemy.mapColider.rightTopY = ((int)enemy.transform.y + enemy.speed.tmpY - (int)enemy.transform.r) / MAP_SIZE;
				enemy.mapColider.leftBottomY = ((int)enemy.transform.y + enemy.speed.tmpY + (int)enemy.transform.r - 1) / MAP_SIZE;
				enemy.mapColider.rightBottomY = ((int)enemy.transform.y + enemy.speed.tmpY + (int)enemy.transform.r - 1) / MAP_SIZE;

				if (enemy.speed.tmpY < 0) {
					if (map[enemy.mapColider.rightTopY][enemy.mapColider.rightTopX] == 0 &&
						map[enemy.mapColider.leftTopY][enemy.mapColider.leftTopX] == 0) {
						enemy.speed.y = enemy.speed.tmpY;
					}
					else {//�V��ɂ���������
						enemy.AirTimer = 0;
						enemy.speed.initialspeedY = 0;
					}
				}

				else if (0 <= enemy.speed.tmpY) {
					if (map[enemy.mapColider.rightBottomY][enemy.mapColider.rightBottomX] == 0 &&
						map[enemy.mapColider.leftBottomY][enemy.mapColider.leftBottomX] == 0) {
						enemy.speed.y = enemy.speed.tmpY;
					}
					else {
						//�n�ʂɂ�����
						enemy.isInTheAir = false;
						enemy.AirTimer = 0;
						enemy.transform.y = enemy.mapColider.rightBottomY * MAP_SIZE - 16;
					}
				}

				//���ۂɓG��i�߂�
				enemy.transform.y += enemy.speed.y;

				//�G�̖{���̎l���̏c�����̍��W���擾����
				enemy.mapColider.leftTopY = ((int)enemy.transform.y - (int)enemy.transform.r) / MAP_SIZE;
				enemy.mapColider.leftBottomY = ((int)enemy.transform.y + (int)enemy.transform.r - 1) / MAP_SIZE;
				enemy.mapColider.rightTopY = ((int)enemy.transform.y - (int)enemy.transform.r) / MAP_SIZE;
				enemy.mapColider.rightBottomY = ((int)enemy.transform.y + (int)enemy.transform.r - 1) / MAP_SIZE;

				enemy.AirTimer++;
			}
		}
	}

	if (0 <= enemy.attackCT) {
		enemy.attackCT--;
	}
	enemy.screen.x = enemy.transform.x - scroll.x;
	enemy.screen.y = enemy.transform.y - scroll.y;

	weapon->Update(enemy.transform.x , enemy.transform.y , enemy.angle , scroll , map);

}

void Enemy::Draw() {

#pragma region//�f�o�b�O�p:�U���͈͂̕\��
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA , 30);
	//DrawCircle(enemy.screen.x ,
	//		   enemy.screen.y ,
	//		   enemy.sensingRenge ,
	//		   GetColor(200 , 0 , 0) ,
	//		   true);

	//DrawCircle(enemy.screen.x ,
	//		   enemy.screen.y ,
	//		   enemy.attackRenge ,
	//		   GetColor(0 , 200 , 0) ,
	//		   true);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND , 30);
#pragma endregion////�f�o�b�O�p:�U���͈͂̕\��

	if (enemy.type == SLIME) {
		DrawOfSlime();
	}
	else if (enemy.type == FENCER) {
		DrawOfFencer();
	}
	else if (enemy.type == ARCHER) {
		DrawOfArcher();
	}

	weapon->Draw();

}


#pragma region//�X���C��

void Enemy::MoveOfSlime(int  map[MAP_HEIGHT][MAP_WIDTH]) {

	if (enemy.isInTheAir == false) {

		if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//�v���C���[���E�[��荶�ɂ���Ƃ�
			if (enemy.isFacingRight == true) {//�E�������Ă��Ȃ�������
				//���݂̃v���C���[�̉E��A�E���̒��_�Ɉ�񕪉E�����ɐi�񂾏ꍇ�̍��W���擾
				enemy.mapColider.rightTopX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
				enemy.mapColider.rightBottomX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
				//�擾�������W�̈ʒu�̃}�b�v�`�b�v��NONE�ł���΃v���C���[�̉������̃X�s�[�h���E�ɍs���悤�ɒl������
				if (map[enemy.mapColider.rightTopY][enemy.mapColider.rightTopX] == NONE && map[enemy.mapColider.rightBottomY][enemy.mapColider.rightBottomX] == NONE) {
					enemy.speed.x += enemy.speed.fixedValue;
				}
			}
		}

		if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//�v���C���[���E�[��荶�ɂ���Ƃ�
			if (enemy.isFacingRight == false) {//�E�������Ȃ�������
				//���݂̃v���C���[�̍���A�����̒��_�Ɉ�񕪍������ɐi�񂾏ꍇ�̍��W���擾
				enemy.mapColider.leftTopX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
				enemy.mapColider.leftBottomX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
				//�擾�������W�̈ʒu�̃}�b�v�`�b�v��NONE�ł���΃v���C���[�̉������̃X�s�[�h�����ɍs���悤�ɒl������
				if (map[enemy.mapColider.leftTopY][enemy.mapColider.leftTopX] == NONE && map[enemy.mapColider.leftBottomY][enemy.mapColider.leftBottomX] == NONE) {
					enemy.speed.x -= enemy.speed.fixedValue;
				}
			}
		}

		//���ۂɃv���C���[��i�߂�
		enemy.transform.x += enemy.speed.x;

		//�v���C���[�̖{���̎l���̉������̍��W���擾����
		enemy.mapColider.rightTopX = (enemy.transform.x + enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.rightBottomX = (enemy.transform.x + enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.leftTopX = (enemy.transform.x - enemy.transform.r) / MAP_SIZE;
		enemy.mapColider.leftBottomX = (enemy.transform.x - enemy.transform.r) / MAP_SIZE;
	}
}

void Enemy::AttackOfSlime(int  map[MAP_HEIGHT][MAP_WIDTH]) {

	if (enemy.motionPhase == 0) {
		enemy.speed.initialspeedY = -3;
		enemy.speed.fixedValue = 5;
		enemy.isInTheAir = true;
		enemy.AirTimer = 0;
		enemy.motionTimer = 25;
		enemy.motionPhase++;
	}

	else if (enemy.motionPhase == 1) {
		enemy.motionTimer--;
		if (enemy.motionTimer <= 0) {
			enemy.motionPhase++;
		}
	}

	else if (enemy.motionPhase == 2) {

		enemy.speed.tmpY = enemy.speed.initialspeedY + (G / 50) * enemy.AirTimer;

		//���݂̃X���C���̎l���̏c�����̍��W�Ɉ�񕪐i�񂾏ꍇ�̍��W���擾
		enemy.mapColider.leftTopY = ((int)enemy.transform.y + enemy.speed.tmpY - (int)enemy.transform.r) / MAP_SIZE;
		enemy.mapColider.rightTopY = ((int)enemy.transform.y + enemy.speed.tmpY - (int)enemy.transform.r) / MAP_SIZE;
		enemy.mapColider.leftBottomY = ((int)enemy.transform.y + enemy.speed.tmpY + (int)enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.rightBottomY = ((int)enemy.transform.y + enemy.speed.tmpY + (int)enemy.transform.r - 1) / MAP_SIZE;

		if (enemy.speed.tmpY < 0) {
			if (map[enemy.mapColider.rightTopY][enemy.mapColider.rightTopX] == 0 &&
				map[enemy.mapColider.leftTopY][enemy.mapColider.leftTopX] == 0) {
				enemy.speed.y = enemy.speed.tmpY;
			}
			//�V��ɓ��������Ƃ�
			else {
				enemy.AirTimer = 0;
				enemy.speed.initialspeedY = 0;
			}
		}

		else if (0 <= enemy.speed.tmpY) {
			if (map[enemy.mapColider.rightBottomY][enemy.mapColider.rightBottomX] == 0 &&
				map[enemy.mapColider.leftBottomY][enemy.mapColider.leftBottomX] == 0) {
				enemy.speed.y = enemy.speed.tmpY;
			}
			//�n�ʂɕt������
			else {
				enemy.isInTheAir = false;
				enemy.AirTimer = 0;
				enemy.transform.y = enemy.mapColider.rightBottomY * MAP_SIZE - 16;
				enemy.motionPhase++;
				enemy.motionTimer = 30;
			}
		}

		//���ۂɃX���C����i�߂�
		enemy.transform.y += enemy.speed.y;

		//�X���C���̖{���̎l���̏c�����̍��W���擾����
		enemy.mapColider.leftTopY = ((int)enemy.transform.y - (int)enemy.transform.r) / MAP_SIZE;
		enemy.mapColider.leftBottomY = ((int)enemy.transform.y + (int)enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.rightTopY = ((int)enemy.transform.y - (int)enemy.transform.r) / MAP_SIZE;
		enemy.mapColider.rightBottomY = ((int)enemy.transform.y + (int)enemy.transform.r - 1) / MAP_SIZE;

		if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//�X���C�����E�[��荶�ɂ���Ƃ�
			if (enemy.isFacingRight == true) {//�E�������Ă�����
				//���݂̃X���C���̉E��A�E���̒��_�Ɉ�񕪉E�����ɐi�񂾏ꍇ�̍��W���擾
				enemy.mapColider.rightTopX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
				enemy.mapColider.rightBottomX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
				//�擾�������W�̈ʒu�̃}�b�v�`�b�v��NONE�ł���΃X���C���̉������̃X�s�[�h���E�ɍs���悤�ɒl������
				if (map[enemy.mapColider.rightTopY][enemy.mapColider.rightTopX] == NONE && map[enemy.mapColider.rightBottomY][enemy.mapColider.rightBottomX] == NONE) {
					enemy.speed.x += enemy.speed.fixedValue;
				}
				else {//�ǂɓ���������
					enemy.transform.x = enemy.mapColider.rightBottomX * MAP_SIZE - 16;
					enemy.speed.fixedValue = 0;
				}
			}
		}

		if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//�X���C�����E�[��荶�ɂ���Ƃ�
			if (enemy.isFacingRight == false) {//�E�������Ȃ�������
				//���݂̃X���C���̍���A�����̒��_�Ɉ�񕪍������ɐi�񂾏ꍇ�̍��W���擾
				enemy.mapColider.leftTopX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
				enemy.mapColider.leftBottomX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
				//�擾�������W�̈ʒu�̃}�b�v�`�b�v��NONE�ł���΃X���C���̉������̃X�s�[�h�����ɍs���悤�ɒl������
				if (map[enemy.mapColider.leftTopY][enemy.mapColider.leftTopX] == NONE && map[enemy.mapColider.leftBottomY][enemy.mapColider.leftBottomX] == NONE) {
					enemy.speed.x -= enemy.speed.fixedValue;
				}
				else {//�ǂɓ���������
					enemy.transform.x = enemy.mapColider.rightBottomX * MAP_SIZE + 15;
					enemy.speed.fixedValue = 0;
				}
			}
		}

		//���ۂɃX���C����i�߂�
		enemy.transform.x += enemy.speed.x;

		//�X���C���̖{���̎l���̉������̍��W���擾����
		enemy.mapColider.rightTopX = ((int)enemy.transform.x + (int)enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.rightBottomX = ((int)enemy.transform.x + (int)enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.leftTopX = ((int)enemy.transform.x - (int)enemy.transform.r) / MAP_SIZE;
		enemy.mapColider.leftBottomX = ((int)enemy.transform.x - (int)enemy.transform.r) / MAP_SIZE;

		enemy.AirTimer++;
	}
	else if (enemy.motionPhase == 3) {
		enemy.motionTimer--;
		if (enemy.motionTimer <= 0) {
			enemy.isAttack = false;
			enemy.motionPhase = 0;
			enemy.attackCT = 5;
			enemy.speed.fixedValue = 1;
		}
	}
}

void Enemy::DrawOfSlime() {
	DrawCircle(enemy.screen.x ,
			   enemy.screen.y ,
			   enemy.transform.r ,
			   GetColor(200 , 50 , 50) ,
			   true);

	if (enemy.isFacingRight == true) {

		DrawBox(enemy.screen.x - enemy.transform.r / 2 , enemy.screen.y + enemy.transform.r ,
				enemy.screen.x , enemy.screen.y - enemy.transform.r ,
				GetColor(200 , 50 , 50) ,
				true);

		DrawTriangle(enemy.screen.x - enemy.transform.r / 2 , enemy.screen.y + enemy.transform.r ,
					 enemy.screen.x - enemy.transform.r / 2 , enemy.screen.y - enemy.transform.r ,
					 enemy.screen.x - enemy.transform.r * 2 , enemy.screen.y + enemy.transform.r ,
					 GetColor(200 , 50 , 50) ,
					 true);
	}

	else {

		DrawBox(enemy.screen.x + enemy.transform.r / 2 , enemy.screen.y + enemy.transform.r ,
				enemy.screen.x , enemy.screen.y - enemy.transform.r ,
				GetColor(200 , 50 , 50) ,
				true);

		DrawTriangle(enemy.screen.x + enemy.transform.r / 2 , enemy.screen.y + enemy.transform.r ,
					 enemy.screen.x + enemy.transform.r / 2 , enemy.screen.y - enemy.transform.r ,
					 enemy.screen.x + enemy.transform.r * 2 , enemy.screen.y + enemy.transform.r ,
					 GetColor(200 , 50 , 50) ,
					 true);
	}
}

#pragma endregion//�X���C��

#pragma region//�t�F���T�[

void Enemy::MoveOfFencer(int  map[MAP_HEIGHT][MAP_WIDTH]) {

	if (enemy.isInTheAir == false) {

		if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//�v���C���[���E�[��荶�ɂ���Ƃ�
			if (enemy.isFacingRight == true) {//�E�������Ă��Ȃ�������
				//���݂̃v���C���[�̉E��A�E���̒��_�Ɉ�񕪉E�����ɐi�񂾏ꍇ�̍��W���擾
				enemy.mapColider.rightTopX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
				enemy.mapColider.rightBottomX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
				//�擾�������W�̈ʒu�̃}�b�v�`�b�v��NONE�ł���΃v���C���[�̉������̃X�s�[�h���E�ɍs���悤�ɒl������
				if (map[enemy.mapColider.rightTopY][enemy.mapColider.rightTopX] == NONE && map[enemy.mapColider.rightBottomY][enemy.mapColider.rightBottomX] == NONE) {
					enemy.speed.x += enemy.speed.fixedValue;
				}
			}
		}

		if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//�v���C���[���E�[��荶�ɂ���Ƃ�
			if (enemy.isFacingRight == false) {//�E�������Ȃ�������
				//���݂̃v���C���[�̍���A�����̒��_�Ɉ�񕪍������ɐi�񂾏ꍇ�̍��W���擾
				enemy.mapColider.leftTopX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
				enemy.mapColider.leftBottomX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
				//�擾�������W�̈ʒu�̃}�b�v�`�b�v��NONE�ł���΃v���C���[�̉������̃X�s�[�h�����ɍs���悤�ɒl������
				if (map[enemy.mapColider.leftTopY][enemy.mapColider.leftTopX] == NONE && map[enemy.mapColider.leftBottomY][enemy.mapColider.leftBottomX] == NONE) {
					enemy.speed.x -= enemy.speed.fixedValue;
				}
			}
		}

		//���ۂɃv���C���[��i�߂�
		enemy.transform.x += enemy.speed.x;

		//�v���C���[�̖{���̎l���̉������̍��W���擾����
		enemy.mapColider.rightTopX = (enemy.transform.x + enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.rightBottomX = (enemy.transform.x + enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.leftTopX = (enemy.transform.x - enemy.transform.r) / MAP_SIZE;
		enemy.mapColider.leftBottomX = (enemy.transform.x - enemy.transform.r) / MAP_SIZE;
	}

	if (enemy.isFacingRight == true) {
		enemy.angle = PI / 6;
	}
	else {
		enemy.angle = PI * 5 / 6;
	}

}

void Enemy::AttackOfFencer(int  map[MAP_HEIGHT][MAP_WIDTH]) {
	if (enemy.motionPhase == 0) {
		if (enemy.distanceToPlayer < enemy.attackRenge / 2) {
			enemy.motionPhase = 4;
		}
		else {
			enemy.motionPhase = 1;
		}
	}
#pragma region//�U���p�^�[��1
	else if (enemy.motionPhase == 1) {
		enemy.speed.tmpX = (enemy.speed.fixedValue + 1) - (0.04 * (double)enemy.motionTimer);

		if (0 < enemy.speed.tmpX) {
			if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//�v���C���[���E�[��荶�ɂ���Ƃ�
				if (enemy.isFacingRight == true) {//�E�������Ă�����
					//���݂̃v���C���[�̉E��A�E���̒��_�Ɉ�񕪉E�����ɐi�񂾏ꍇ�̍��W���擾
					enemy.mapColider.rightTopX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
					enemy.mapColider.rightBottomX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
					//�擾�������W�̈ʒu�̃}�b�v�`�b�v��NONE�ł���΃v���C���[�̉������̃X�s�[�h���E�ɍs���悤�ɒl������
					if (map[enemy.mapColider.rightTopY][enemy.mapColider.rightTopX] == NONE && map[enemy.mapColider.rightBottomY][enemy.mapColider.rightBottomX] == NONE) {
						enemy.speed.x += enemy.speed.tmpX;
					}
					enemy.angle -= (PI * 4 / 6) / 75;
				}
			}

			if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//�v���C���[���E�[��荶�ɂ���Ƃ�
				if (enemy.isFacingRight == false) {//�E�������Ȃ�������
					//���݂̃v���C���[�̍���A�����̒��_�Ɉ�񕪍������ɐi�񂾏ꍇ�̍��W���擾
					enemy.mapColider.leftTopX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
					enemy.mapColider.leftBottomX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
					//�擾�������W�̈ʒu�̃}�b�v�`�b�v��NONE�ł���΃v���C���[�̉������̃X�s�[�h�����ɍs���悤�ɒl������
					if (map[enemy.mapColider.leftTopY][enemy.mapColider.leftTopX] == NONE && map[enemy.mapColider.leftBottomY][enemy.mapColider.leftBottomX] == NONE) {
						enemy.speed.x -= enemy.speed.tmpX;
					}
					enemy.angle += (PI * 4 / 6) / 75;
				}
			}

			//���ۂɃv���C���[��i�߂�
			enemy.transform.x += enemy.speed.x;

			//�v���C���[�̖{���̎l���̉������̍��W���擾����
			enemy.mapColider.rightTopX = (enemy.transform.x + enemy.transform.r - 1) / MAP_SIZE;
			enemy.mapColider.rightBottomX = (enemy.transform.x + enemy.transform.r - 1) / MAP_SIZE;
			enemy.mapColider.leftTopX = (enemy.transform.x - enemy.transform.r) / MAP_SIZE;
			enemy.mapColider.leftBottomX = (enemy.transform.x - enemy.transform.r) / MAP_SIZE;
		}

		else {
			enemy.motionPhase++;
			enemy.motionTimer = 0;
		}

		enemy.motionTimer++;
	}

	else if (enemy.motionPhase == 2) {
		if (enemy.motionTimer <= 4) {
			if (enemy.isFacingRight == true) {
				enemy.angle += (PI / 6);
			}
			else {
				enemy.angle -= (PI / 6);
			}
			enemy.motionTimer++;
		}
		else {
			enemy.motionTimer = 40;
			enemy.motionPhase++;
		}
	}
	else if (enemy.motionPhase == 3) {
		if (0 <= enemy.motionTimer) {
			enemy.motionTimer--;
		}
		else {
			enemy.motionPhase = 0;
			enemy.motionTimer = 0;
			enemy.isAttack = false;
			enemy.attackCT = 5;
		}
	}
#pragma endregion//�U���p�^�[��1

#pragma region//�U���p�^�[��2

	else if (enemy.motionPhase == 4) {
		if (enemy.motionTimer <= 6) {
			if (enemy.isFacingRight == true) {
				enemy.angle -= (PI / 12);
			}
			else {
				enemy.angle += (PI / 12);
			}
			enemy.motionTimer++;
		}
		else {
			enemy.motionTimer = 0;
			enemy.motionPhase++;
		}
	}

	else if (enemy.motionPhase == 5) {
		if (enemy.motionTimer <= 6) {
			if (enemy.isFacingRight == true) {
				enemy.angle += (PI / 12);
			}
			else {
				enemy.angle -= (PI / 12);
			}
			enemy.motionTimer++;
		}
		else {
			enemy.motionTimer = 0;
			enemy.motionPhase++;
		}
	}

	else if (enemy.motionPhase == 6) {
		if (enemy.motionTimer <= 6) {
			if (enemy.isFacingRight == true) {
				enemy.angle -= (PI / 12);
			}
			else {
				enemy.angle += (PI / 12);
			}
			enemy.motionTimer++;
		}
		else {
			enemy.motionTimer = 0;
			enemy.motionPhase++;
		}
	}

	else if (enemy.motionPhase == 7) {
		if (enemy.motionTimer <= 6) {
			if (enemy.isFacingRight == true) {
				enemy.angle += (PI / 12);
			}
			else {
				enemy.angle -= (PI / 12);
			}
			enemy.motionTimer++;
		}
		else {
			enemy.motionTimer = 0;
			enemy.motionPhase++;
		}
	}

	else if (enemy.motionPhase == 8) {
		if (enemy.motionTimer <= 6) {
			if (enemy.isFacingRight == true) {
				enemy.angle -= (PI / 12);
			}
			else {
				enemy.angle += (PI / 12);
			}
			enemy.motionTimer++;
		}
		else {
			enemy.motionTimer = 50;
			enemy.motionPhase++;
		}
	}

	else if (enemy.motionPhase == 9) {
		if (0 <= enemy.motionTimer) {
			enemy.motionTimer--;
			if (enemy.motionTimer <= 30) {
				if (enemy.isFacingRight == true) {
					enemy.angle += (PI * 6 / 12) / 28;
				}
				else {
					enemy.angle -= (PI * 6 / 12) / 28;
				}
			}
		}
		else {
			enemy.motionPhase = 0;
			enemy.motionTimer = 0;
			enemy.isAttack = false;
			enemy.attackCT = 5;
		}
	}

#pragma endregion//�U���p�^�[��2

}

void Enemy::DrawOfFencer() {

	DrawCircle(enemy.screen.x ,
			   enemy.screen.y ,
			   enemy.transform.r ,
			   GetColor(200 , 50 , 50) ,
			   true);

}

#pragma endregion//�t�F���T�[

#pragma region//�A�[�`���[

void Enemy::MoveOfArcher(int  map[MAP_HEIGHT][MAP_WIDTH] , Player player) {

	if (enemy.isFacingRight == true) {
		enemy.angle = 0;
	}
	else {
		enemy.angle = PI;
	}

}

void Enemy::AttackOfArcher(int  map[MAP_HEIGHT][MAP_WIDTH] , Player player) {

	if (enemy.motionPhase == 0) {
		enemy.motionTimer = 50;
		enemy.motionPhase++;
	}

	else if (enemy.motionPhase == 1) {
		enemy.angle = atan2((double)player.GetPlyaerTransformY() - enemy.transform.y ,
							(double)player.GetPlyaerTransformX() - enemy.transform.x);

		if (0 < enemy.motionTimer) {
			enemy.motionTimer--;
		}
		else {
			enemy.motionPhase++;
			enemy.motionTimer = 200;
			enemy.isShotArrow = true;
			weapon->SetIsShot(enemy.isShotArrow);
		}
	}

	else if (enemy.motionPhase == 2) {

		if (enemy.isShotArrow == true) {
			if (0 < enemy.motionTimer) {
				enemy.motionTimer--;
			}
			else {
				enemy.isShotArrow = false;
				weapon->SetIsShot(enemy.isShotArrow);
			}
		}
		else if (enemy.isShotArrow == false) {
			enemy.motionPhase = 0;
			enemy.attackCT = 100;
			enemy.isAttack = false;
		}

	}

}

void Enemy::DrawOfArcher() {

	DrawCircle(enemy.screen.x ,
			   enemy.screen.y ,
			   enemy.transform.r ,
			   GetColor(200 , 50 , 50) ,
			   true);

	DrawTriangle(enemy.screen.x + cos(enemy.angle) * enemy.transform.r * 2 ,
				 enemy.screen.y + sin(enemy.angle) * enemy.transform.r * 2 ,
				 enemy.screen.x + cos(enemy.angle) * enemy.transform.r + cos(enemy.angle + PI / 2) * enemy.transform.r ,
				 enemy.screen.y + sin(enemy.angle) * enemy.transform.r + sin(enemy.angle + PI / 2) * enemy.transform.r ,
				 enemy.screen.x + cos(enemy.angle) * enemy.transform.r - cos(enemy.angle + PI / 2) * enemy.transform.r ,
				 enemy.screen.y + sin(enemy.angle) * enemy.transform.r - sin(enemy.angle + PI / 2) * enemy.transform.r ,
				 GetColor(200 , 50 , 50) ,
				 false);

}

#pragma endregion//�A�[�`���[