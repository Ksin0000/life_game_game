#include "DxLib.h"
#include <string>
#include <stdio.h>	// �t�@�C������ɕK�v�ȃw�b�_�t�@�C��

//�O���[�o���ϐ��̐錾
const int map_size_x = 21, map_size_y = 17;
bool field[map_size_x][map_size_y]; //�t�B�[���h
bool cell[map_size_x][map_size_y]; //�Z���B�t�B�[���h�Əd�ˍ��킹�Ďg�p�B

int getrand() //1/N�̊m����1��Ԃ�
{
	int N = 3;
	int temp = GetRand(N);

	if (temp == 0)return 1;
	else return 0;
}

bool sibouhanntei(int p_x, int p_y)
{
	if (cell[p_x][p_y] == 1) {
		return 1;
	}
	return 0;
}

void life_update()//���C�t�Q�[������
{
	int sum = 0;//���̃Z���̎���ɉ����C�t�����邩
	int temp[map_size_x][map_size_y];
	for (int i = 0; i < map_size_x; i++)
	{
		for (int j = 0; j < map_size_y; j++) 
		{
			temp[i][j] = cell[i][j];
			//�[�����̃Z���������Ă̏���������
			if ((i != 0) && (j != 0) && (i != map_size_x-1) && (j != map_size_y-1))
			{
				for (int k = -1; k <= 1; k++) 
				{
					for (int l = -1; l <= 1; l++) 
					{
						if ((k != 0) || (l != 0)) 
						{
							sum += cell[i + k][j + l];
						}
					}
				}
				if (cell[i][j]) 
				{			
					if ((sum != 2) && (sum != 3)) //�Z���̎����2��3�̃Z��������ΐ���
					{
						temp[i][j] = 0;
					}
				}	
				else if (!cell[i][j]) //�Z���̎����3�̃Z����������a��
				{
					if (sum == 3) 
					{
						temp[i][j] = 1;
					}
				}
				else //����ȊO�͎���
				{
					temp[i][j] = 0;
				}
			}
			sum = 0;
		}
	}
	//���̌��ʂ�cell�ɑ������
	for (int i = 1; i < map_size_x-1; i++) {
		for (int j = 1; j < map_size_y-1; j++) {
			cell[i][j] = temp[i][j];
		}
	}
}

void life_down() 
{
	int temp[map_size_x][map_size_y] = {};

	for (int i = 1; i < map_size_x; i++)
	{
		for (int j = 1; j < map_size_y-1; j++)
		{
			temp[i][j+1] = cell[i][j];
		}
	}
	
	//���̌��ʂ�cell�ɑ������
	for (int i = 0; i < map_size_x; i++) {
		for (int j = 0; j < map_size_y; j++) {
			cell[i][j] = temp[i][j];
		}
	}
}

void life_clear()
{
	for (int i = 0; i < map_size_x; i++) {
		for (int j = 0; j < map_size_y; j++) {
			cell[i][j] = 0;
		}
	}
}

void life_make()
{
	for (int i = 1; i < map_size_x-1; i++){
		for (int j = 1; j <= 5; j++)
		{
			cell[i][j] = getrand();
		}
	}
}

int point_cal(int p_x, int p_y) 
{
	int point_temp = 0;
	int a = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (cell[p_x + i][p_y + j])
			{
				if ((a % 2 == 0) && (a != 4))
				{
					point_temp += 10;
				}
				else if ((a % 2 == 1) && (a != 4))
				{
					point_temp += 20;
				}
			}
			a++;
		}
	}
	return point_temp;
}

void HiScoreSave(int HiScore)
{
	// �n�C�X�R�A�Z�[�u�p�t�@�C�����J��
	// (�Q�Ԗڂ̈�����"wb"��'w'�́u�������݁v�̈Ӗ�
	FILE *fout;
	fopen_s(&fout,"HiScore.txt", "w");

	// �I�[�v���ł��Ȃ������炱���ŏI��
	if (fout == NULL) return;

	// �n�C�X�R�A�f�[�^�̏����o��
	//fwrite(&HiScore, sizeof(HiScore), 1, fp);
	fprintf_s(fout, "%d", HiScore);

	// �t�@�C�������
	fclose(fout);
}

int HiScoreload() {
	//�Z�[�u�f�[�^�ǂݍ���
	int hiscore = 0;
	FILE *fread;//�t�@�C���ǂݍ���
	errno_t error;
	error = fopen_s(&fread, "HiScore.txt", "r");
	if (error == 0)
	{
		fscanf_s(fread, "%d", &hiscore);
	}
	fclose(fread);// �t�@�C�������

	return hiscore;
}

void howtoplay() 
{
	//char Buf[256];//GetHitKeyStateAll�̂��
	unsigned int Cr = GetColor(255, 255, 255);//��
	const int gyou = 20;

	//SetBackgroundColor(0, 0, 0);
	//GetHitKeyStateAll(Buf);

	std::string str[gyou] =
	{
		"       --�V�ѕ�--",
		"�v���C���[�͓_P�ł��B",
		"�ނ͉E�A�^�񒆁A���ɓ����܂��B",
		"���C�t�Q�[���ɂ�����Ɠ��_�ł��B",
		"�㉺���E��20�_�A�ȂȂ߂�10�_�A��i�ނ�1�_�ł��B",
		"��x�������тɃ��C�t�Q�[����1�^�[���i��1�}�X�����Ă��܂��B",
		"���C�t�Q�[���Ɉ��ݍ��܂�邩�ړ���j�܂��Ǝ���ł��܂��܂��B",
		"���C�t�Q�[���̓�����\�����Ȃ���i��ōs���Ă��������B",
		"",
		"���C�t�Q�[���͉��̗l�Ȗ@���œ����܂��B",
		"1.���̃}�X���΂̏ꍇ�A",
		"�@����8�}�X�̂���2�}�X��3�}�X���΂Ȃ炻�̃}�X�͗΂̂܂܁i�u�����v�j",
		"2.���̃}�X�����̏ꍇ�A",
		"�@����8�}�X�̂���3�}�X���΂Ȃ炻�̃}�X���΂ɂ���i�u�a���v�j",
		"3.����ȊO�̏ꍇ�A",
		"�@���̃}�X�͉��F�ł��i���ł��΂ł��j���ɂ��Ă���",
		"�@�i�����΂������ꍇ�́A�u���S�v�j",
		"",
		"",
		"",
	};

	for (int i = 0; i < gyou; i++)
	{
		DrawFormatString(8, i * 16+8, Cr, str[i].c_str());
	}
}

void game_main()
{
	char Buf[256];//GetHitKeyStateAll�̂��
	int key_count = 0;

	//�����z�u
	life_make();

	const int masu = 16;
	bool turn = false;//�^�[����1�̂Ƃ��@�B���̑���
	int p_x = map_size_x / 2;//�v���C���[�̈ʒu
	int p_y = map_size_y - 1;
	const int turn_time = 10;//�@�B���̃^�[���͉��b�t���[����
	const int idou_s = turn_time + 10;//��������10�t���[���Ɉ��i��
	int turn_count = 0;//�@�B���̃^�[���𐔂���

	int down_count = 0;//life_down��������s������ ���^�[���ڂ�

	int point = 0;// �ŏI�I�ȓ��_�� point + down_count
	int point_temp = 0;//���݂̃^�[���ŐV���ɂƂ������_

	int hiscore = HiScoreload();//���݂̃n�C�X�R�A

	unsigned int Color_b = GetColor(0, 0, 0);//��
	unsigned int Color_w = GetColor(255, 255, 255);//��
	unsigned int Color_g = GetColor(0, 255, 0);//��
	unsigned int Color_p = GetColor(240, 240, 240);//�_P�̐F
	unsigned int Color_waku = GetColor(255 / 4, 255 / 4, 255 / 4);//�g�̐F


	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		SetBackgroundColor(0, 0, 0);
		GetHitKeyStateAll(Buf);

		//�g�\��
		DrawBox(masu/2, masu / 2, map_size_x*masu- masu , (map_size_y+1)*masu , Color_waku, TRUE);//�O�g
		//DrawBox(masu - 1, masu - 1, (map_size_x-1)*masu + 1, map_size_y*masu + 1, Color_b, TRUE);//���g�i�h��Ԃ��j
		//DrawBox(masu - 1, (map_size_y - 11)*masu + 1, masu * 2 - 2, map_size_y *masu + 1, Color_waku, TRUE);
		//DrawBox(masu*(map_size_x - 1) + 1, (map_size_y - 11)*masu + 1, masu *(map_size_x)+1, map_size_y *masu + 1, Color_waku, TRUE);
		//DrawBox(masu / 2, (map_size_y - 11)*masu + masu / 2, masu * 2-masu/2, (map_size_y+1) *masu + 1, Color_b, TRUE);
		//DrawBox(masu*(map_size_x-1)+masu/2,  (map_size_y - 11)*masu + masu / 2, masu *(map_size_x+1), (map_size_y + 1) *masu + 1, Color_b, TRUE);
		

		//�o�\��
		//DrawGraph(p_x*masu, p_y*masu, ziki, FALSE);
		DrawCircle(p_x*masu+masu/2, p_y*masu + masu / 2,masu*2/16, Color_w, TRUE);
		//life�\��
		for (int i = 0; i < map_size_x; i++)
		{
			for (int j = 0; j <= map_size_y; j++)
			{
				field[i][j] = cell[i][j];
				if (field[i][j]/* == 1*/) {
					//DrawGraph(i*masu, j*masu, block, FALSE);
					DrawBox(i*masu, j*masu, (i+1)*masu-1, (j+1)*masu-1, Color_g, TRUE);

				}
			}
		}

		//���_�\��
		DrawFormatString((map_size_x + 1) * 16, (1) * 16, Color_w, "High score %d", hiscore);
		DrawFormatString((map_size_x + 1) * 16, (map_size_y - 3) * 16, Color_w, "POINT �O�̃^�[��+�ǉ����_");
		DrawFormatString((map_size_x + 1) * 16, (map_size_y - 2) * 16, Color_w, "POINT: %d + %d", point + down_count - point_temp - !(down_count == 0), point_temp + 1);
		DrawFormatString((map_size_x + 1) * 16, (map_size_y - 1) * 16, Color_w, "      =%d", point + down_count);

		if (Buf[KEY_INPUT_ESCAPE] == 1) {
			break;
		}

		//�ړ��悠�邩�ǂ���
		bool lgo = false, rgo = false, ugo = false;//�ړI�n�ɓ��B�\�Ȃ�true
		if (cell[p_x - 1][p_y - 1] == 0 && 1 <= p_x - 1) {
			lgo = true;
		}
		if (cell[p_x][p_y - 1] == 0) {
			ugo = true;
		}
		if (cell[p_x + 1][p_y - 1] == 0 && p_x + 1 < map_size_x - 1) {
			rgo = true;
		}

		int yazirusi_y = (map_size_y + 2) * 16;
		//���̂�[��
		if (lgo) {
			DrawFormatString((p_x - 1) * 16, yazirusi_y, Color_w, "��", point + down_count);
			DrawBox((p_x - 1) * 16, yazirusi_y, (p_x) * 16, yazirusi_y+masu, Color_w, FALSE);
		}
		if (ugo) {
			DrawFormatString((p_x) * 16, yazirusi_y, Color_w, "��", point + down_count);
			DrawBox((p_x) * 16, yazirusi_y, (p_x + 1) * 16, yazirusi_y + masu, Color_w, FALSE);
		}
		if (rgo) {
			DrawFormatString((p_x + 1) * 16, yazirusi_y, Color_w, "��", point + down_count);
			DrawBox((p_x + 1) * 16, yazirusi_y, (p_x + 2) * 16, yazirusi_y + masu, Color_w, FALSE);
		}


		//�ړ��E�ƍ��Ɨ��܂�  �^�[�����i�ރg���K�[
		if (Buf[KEY_INPUT_A] == 1 || Buf[KEY_INPUT_LEFT] == 1)
		{
			if (key_count == 0) {
				if (lgo)
				{
					p_x--;
					p_y--;
					turn++;
				}
				key_count++;
			}
			else
			{
				key_count++;
				if (idou_s < key_count)
				{
					key_count = 0;
				}
			}
		}
		else if (Buf[KEY_INPUT_D] == 1 || Buf[KEY_INPUT_RIGHT] == 1)
		{
			if (key_count == 0) {
				if (rgo)
				{
					p_x++;
					p_y--;

					turn = true;
				}
				key_count++;
			}
			else
			{
				key_count++;
				if (idou_s < key_count)
				{
					key_count = 0;
				}
			}

		}
		else if (Buf[KEY_INPUT_W] == 1 || Buf[KEY_INPUT_UP] == 1)
		{
			if (key_count == 0) {
				if (ugo)
				{
					p_y--;
					turn = true;
				}
				key_count++;
			}
			else
			{
				key_count++;
				if (idou_s < key_count)
				{
					key_count = 0;
				}
			}

		}

		if (turn == true) //�@�B���̑���
		{
			turn_count++;
			if (turn_count == turn_time*2/4)
			{
				life_update();
			}
			if (turn_count == turn_time*3/4)
			{
				if (sibouhanntei(p_x, p_y))break;//���Ȃ킿��

				point_temp = point_cal(p_x, p_y);
				point += point_temp;
			}
			if (turn_count == turn_time)
			{
				life_down();
				p_y++;

				down_count++;
				if (down_count % 5 == 0) {
					life_make();
				}
				if (!lgo && !rgo && !ugo)//�ړ���Ȃ����
				{
					break;//���Ȃ킿��
				}
				turn = false;
				turn_count = 0;
			}
		}

		if ((CheckHitKeyAll() == false) && (turn != 1/*�L�[���͂̎ז�*/)) {
			key_count = 0;
		}
	}

	//unsigned int Color;
	//Color = GetColor(0, 0, 0);
	DrawFormatString(p_x * 16, p_y * 16, Color_w, "��");


	if (hiscore < (point + down_count)) {//�n�C�X�R�A�X�V
		HiScoreSave(point + down_count);
	}

	WaitKey();
	life_clear();//�}�b�v������
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	ChangeWindowMode(TRUE); // �E�B���h�E���[�h�ɐݒ�
	const int Windowsize_x = 600;
	const int Windowsize_y = 400;
	SetGraphMode(Windowsize_x, Windowsize_y, 32);
	SetMainWindowText("life game game");
	//SetWindowSizeChangeEnableFlag(TRUE);
	DxLib_Init();   // DX���C�u��������������
	char Buf[256];//GetHitKeyStateAll�̂��
	int key_count = 0;

	const int N = 3;//���󂵂̌�
	int selected = 0;//���󂳂�Ă���͉̂��Ԗڂ�
	const int select_s = 6;//����̕ς�鑬��
	unsigned int Color[2] = { GetColor(255,255,255), GetColor(100, 100, 100) };//���󂳂�Ă���z��0�@�@
	bool c[N] = { 0,1,1 };//���󂳂�Ă���z��0
	std::string select_str[N] = { "Game start" , "How to play", "Exit" };
	unsigned int Color_w =  GetColor(255, 255, 255);//���F

	//�G���^�[�̓��͂���A���ł͎󂯕t���Ȃ����߂̕ϐ��@�Q�[���Ŏ�������G���^�[�����Ƃ܂��n�܂����Ⴄ
	bool flag = false;
	int hiscore = HiScoreload();//���݂̃n�C�X�R�A

    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		SetBackgroundColor(0, 0, 0);
		GetHitKeyStateAll(Buf);
		DrawFormatString((map_size_x + 1) * 16, 1 * 16, Color_w, "High score %d",hiscore);

		DrawFormatString(Windowsize_x / 9 - 20, Windowsize_y / 2 + 120, Color_w, "��");
		int select_zure[3] = {};
		for (int i = 0; i < N; i++)
		{
			if (c[i] == 0) {
				select_zure[0] = i * 20;
				select_zure[1] = (i-1) * 20;
				select_zure[2] = (i-2) * 20;
			}
		}
		for  (int i = 0; i < N; i++)
		{
			DrawFormatString(Windowsize_x / 9, Windowsize_y / 2 + 120-select_zure[i], Color[c[i]], select_str[i].c_str());
		}
		
		if (Buf[KEY_INPUT_W] == 1 || Buf[KEY_INPUT_UP] == 1)
		{
			if (key_count == 0) {
				c[selected] = 1;
				selected--;
				if (selected < 0) {
					selected += N;
				}
				c[selected] = 0;
				key_count++;
			}
			else
			{
				key_count++;
				if (select_s<key_count)
				{
					key_count = 0;
				}
			}
		}
		else if (Buf[KEY_INPUT_S] == 1 || Buf[KEY_INPUT_DOWN] == 1)
		{
			if (key_count == 0) {
				c[selected] = 1;
				selected++;
				if (N <= selected) {
					selected -= N;
				}
				c[selected] = 0;
				key_count++;
			}
			else
			{
				key_count++;
				if (select_s<key_count)
				{
					key_count = 0;
	
				}
			}
		}

		if (Buf[KEY_INPUT_RETURN] == 1&&flag==false) {
			if (selected == 0) {
				game_main();
				hiscore = HiScoreload();
			}else if (selected == 1) {
				ClearDrawScreen();
				howtoplay();
				WaitKey();
			}
			else if(selected == 2) {
				break;
			}
			flag = true;
		}

		if (Buf[KEY_INPUT_RETURN] != 1)
			flag = false;

		if (Buf[KEY_INPUT_ESCAPE] == 1) {
			break;
		}

		if (CheckHitKeyAll() == false) {
			key_count = 0;
		}
	}
	DxLib_End();   // DX���C�u�����I������
	return 0;
}