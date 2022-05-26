#include "Player.h"

//�R���X�g���N�^�̒�`
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

//�f�X�g���N�^�̒�`
Player::~Player() {

}

//�����o�֐�
//�X�V�����̊֐�
void Player::Update(char* keys , char* oldkeys , int  map[MAP_HEIGHT][MAP_WIDTH] , Scroll& scroll) {
	player.speed.x = 0;
	player.speed.y = 0;
	player.speed.tmpX = player.speed.fixedValue;
	player.speed.tmpY = 0;

	//�v���C���[�̖{���̎l���̏c�����̍��W���擾����
	player.mapColider.leftTopY = ((int)player.transform.y - (int)player.transform.r) / MAP_SIZE;
	player.mapColider.leftBottomY = ((int)player.transform.y + (int)player.transform.r - 1) / MAP_SIZE;
	player.mapColider.rightTopY = ((int)player.transform.y - (int)player.transform.r) / MAP_SIZE;
	player.mapColider.rightBottomY = ((int)player.transform.y + (int)player.transform.r - 1) / MAP_SIZE;

	//�v���C���[�̖{���̎l���̉������̍��W���擾����
	player.mapColider.rightTopX = ((int)player.transform.x + (int)player.transform.r - 1) / MAP_SIZE;
	player.mapColider.rightBottomX = ((int)player.transform.x + (int)player.transform.r - 1) / MAP_SIZE;
	player.mapColider.leftTopX = ((int)player.transform.x - (int)player.transform.r) / MAP_SIZE;
	player.mapColider.leftBottomX = ((int)player.transform.x - (int)player.transform.r) / MAP_SIZE;

	if (player.isInTheAir == false) {

		if (player.transform.x + player.transform.r < MAP_WIDTH * MAP_SIZE) {//�v���C���[���E�[��荶�ɂ���Ƃ�
			if (keys[KEY_INPUT_RIGHT] == 1 || keys[KEY_INPUT_D] == 1) {//�E�L�[�������ꂽ��
				//���݂̃v���C���[�̉E��A�E���̒��_�Ɉ�񕪉E�����ɐi�񂾏ꍇ�̍��W���擾
				player.mapColider.rightTopX = (player.transform.x + player.speed.tmpX + player.transform.r - 1) / MAP_SIZE;
				player.mapColider.rightBottomX = (player.transform.x + player.speed.tmpX + player.transform.r - 1) / MAP_SIZE;
				//�擾�������W�̈ʒu�̃}�b�v�`�b�v��NONE�ł���΃v���C���[�̉������̃X�s�[�h���E�ɍs���悤�ɒl������
				if (map[player.mapColider.rightTopY][player.mapColider.rightTopX] == NONE && map[player.mapColider.rightBottomY][player.mapColider.rightBottomX] == NONE) {
					player.speed.x += player.speed.fixedValue;
					if (MAP_SIZE * 11 <= player.screen.x) {//�v���C���[��15�}�X�ڈȍ~�ɂ���Ƃ�
						if (scroll.x < scroll.MAX_X) {//�X�N���[���l���ő�l��菬�������
							scroll.x += player.speed.fixedValue;//�v���[���[�̃X�s�[�h���X�N���[���l�𑝂₷
						}
					}
				}
			}
		}

		if (player.transform.x + player.transform.r < MAP_WIDTH * MAP_SIZE) {//�v���C���[���E�[��荶�ɂ���Ƃ�
			if (keys[KEY_INPUT_LEFT] == 1 || keys[KEY_INPUT_A] == 1) {//�E�L�[�������ꂽ��
				//���݂̃v���C���[�̍���A�����̒��_�Ɉ�񕪍������ɐi�񂾏ꍇ�̍��W���擾
				player.mapColider.leftTopX = (player.transform.x - player.speed.tmpX - player.transform.r) / MAP_SIZE;
				player.mapColider.leftBottomX = (player.transform.x - player.speed.tmpX - player.transform.r) / MAP_SIZE;
				//�擾�������W�̈ʒu�̃}�b�v�`�b�v��NONE�ł���΃v���C���[�̉������̃X�s�[�h�����ɍs���悤�ɒl������
				if (map[player.mapColider.leftTopY][player.mapColider.leftTopX] == NONE && map[player.mapColider.leftBottomY][player.mapColider.leftBottomX] == NONE) {
					player.speed.x -= player.speed.fixedValue;
					if (player.screen.x <= MAP_SIZE * 9) {//�v���C���[��5�}�X�ڈȑO�ɂ���Ƃ�
						if (0 < scroll.x) {//�X�N���[���l��0���傫�����
							scroll.x -= player.speed.fixedValue;//�v���[���[�̃X�s�[�h���X�N���[���l�����炷
						}
					}
				}
			}
		}

		//���ۂɃv���C���[��i�߂�
		player.transform.x += player.speed.x;

		//�v���C���[�̖{���̎l���̉������̍��W���擾����
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

		//���݂̃v���C���[�̎l���̏c�����̍��W�Ɉ�񕪐i�񂾏ꍇ�̍��W���擾
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

		//���ۂɃv���C���[��i�߂�
		player.transform.y += player.speed.y;

		//�v���C���[�̖{���̎l���̏c�����̍��W���擾����
		player.mapColider.leftTopY = ((int)player.transform.y - (int)player.transform.r) / MAP_SIZE;
		player.mapColider.leftBottomY = ((int)player.transform.y + (int)player.transform.r - 1) / MAP_SIZE;
		player.mapColider.rightTopY = ((int)player.transform.y - (int)player.transform.r) / MAP_SIZE;
		player.mapColider.rightBottomY = ((int)player.transform.y + (int)player.transform.r - 1) / MAP_SIZE;

		if (player.transform.x + player.transform.r < MAP_WIDTH * MAP_SIZE) {//�v���C���[���E�[��荶�ɂ���Ƃ�
			if (keys[KEY_INPUT_RIGHT] == 1 || keys[KEY_INPUT_D] == 1) {//�E�L�[�������ꂽ��
				//���݂̃v���C���[�̉E��A�E���̒��_�Ɉ�񕪉E�����ɐi�񂾏ꍇ�̍��W���擾
				player.mapColider.rightTopX = (player.transform.x + player.speed.tmpX + player.transform.r - 1) / MAP_SIZE;
				player.mapColider.rightBottomX = (player.transform.x + player.speed.tmpX + player.transform.r - 1) / MAP_SIZE;
				//�擾�������W�̈ʒu�̃}�b�v�`�b�v��NONE�ł���΃v���C���[�̉������̃X�s�[�h���E�ɍs���悤�ɒl������
				if (map[player.mapColider.rightTopY][player.mapColider.rightTopX] == NONE && map[player.mapColider.rightBottomY][player.mapColider.rightBottomX] == NONE) {
					player.speed.x += player.speed.fixedValue;
					if (MAP_SIZE * 11 <= player.screen.x) {//�v���C���[��15�}�X�ڈȍ~�ɂ���Ƃ�
						if (scroll.x < scroll.MAX_X) {//�X�N���[���l���ő�l��菬�������
							scroll.x += player.speed.fixedValue;//�v���[���[�̃X�s�[�h���X�N���[���l�𑝂₷
						}
					}
				}
			}
		}

		if (player.transform.x + player.transform.r < MAP_WIDTH * MAP_SIZE) {//�v���C���[���E�[��荶�ɂ���Ƃ�
			if (keys[KEY_INPUT_LEFT] == 1 || keys[KEY_INPUT_A] == 1) {//�E�L�[�������ꂽ��
				//���݂̃v���C���[�̍���A�����̒��_�Ɉ�񕪍������ɐi�񂾏ꍇ�̍��W���擾
				player.mapColider.leftTopX = (player.transform.x - player.speed.tmpX - player.transform.r) / MAP_SIZE;
				player.mapColider.leftBottomX = (player.transform.x - player.speed.tmpX - player.transform.r) / MAP_SIZE;
				//�擾�������W�̈ʒu�̃}�b�v�`�b�v��NONE�ł���΃v���C���[�̉������̃X�s�[�h�����ɍs���悤�ɒl������
				if (map[player.mapColider.leftTopY][player.mapColider.leftTopX] == NONE && map[player.mapColider.leftBottomY][player.mapColider.leftBottomX] == NONE) {
					player.speed.x -= player.speed.fixedValue;
					if (player.screen.x <= MAP_SIZE * 9) {//�v���C���[��5�}�X�ڈȑO�ɂ���Ƃ�
						if (0 < scroll.x) {//�X�N���[���l��0���傫�����
							scroll.x -= player.speed.fixedValue;//�v���[���[�̃X�s�[�h���X�N���[���l�����炷
						}
					}
				}
			}
		}

		//���ۂɃv���C���[��i�߂�
		player.transform.x += player.speed.x;

		//�v���C���[�̖{���̎l���̉������̍��W���擾����
		player.mapColider.rightTopX = ((int)player.transform.x + (int)player.transform.r - 1) / MAP_SIZE;
		player.mapColider.rightBottomX = ((int)player.transform.x + (int)player.transform.r - 1) / MAP_SIZE;
		player.mapColider.leftTopX = ((int)player.transform.x - (int)player.transform.r) / MAP_SIZE;
		player.mapColider.leftBottomX = ((int)player.transform.x - (int)player.transform.r) / MAP_SIZE;

		player.AirTimer++;
	}

	player.screen.x = player.transform.x - scroll.x;
	player.screen.y = player.transform.y - scroll.y;
}

//�`�揈���̊֐�
void Player::Draw() {
	DrawCircle(player.screen.x ,
			   player.screen.y ,
			   player.transform.r ,
			   GetColor(255 , 255 , 255) ,
			   true);
}

//�Q�b�^�[
double Player::GetPlyaerTransformX() {
	return player.transform.x;
}

double Player::GetPlyaerTransformY() {
	return player.transform.y;
}