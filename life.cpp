#include "DxLib.h"
#include <string>
#include <stdio.h>	// ファイル操作に必要なヘッダファイル

//グローバル変数の宣言
const int map_size_x = 21, map_size_y = 17;
bool field[map_size_x][map_size_y]; //フィールド
bool cell[map_size_x][map_size_y]; //セル。フィールドと重ね合わせて使用。

int getrand() //1/Nの確立で1を返す
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

void life_update()//ライフゲーム部分
{
	int sum = 0;//そのセルの周りに何個ライフがあるか
	int temp[map_size_x][map_size_y];
	for (int i = 0; i < map_size_x; i++)
	{
		for (int j = 0; j < map_size_y; j++) 
		{
			temp[i][j] = cell[i][j];
			//端っこのセルを除いての処理をする
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
					if ((sum != 2) && (sum != 3)) //セルの周りに2つか3つのセルがあれば生存
					{
						temp[i][j] = 0;
					}
				}	
				else if (!cell[i][j]) //セルの周りに3つのセルがいたら誕生
				{
					if (sum == 3) 
					{
						temp[i][j] = 1;
					}
				}
				else //それ以外は死滅
				{
					temp[i][j] = 0;
				}
			}
			sum = 0;
		}
	}
	//この結果をcellに代入する
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
	
	//この結果をcellに代入する
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
	// ハイスコアセーブ用ファイルを開く
	// (２番目の引数の"wb"の'w'は「書きこみ」の意味
	FILE *fout;
	fopen_s(&fout,"HiScore.txt", "w");

	// オープンできなかったらここで終了
	if (fout == NULL) return;

	// ハイスコアデータの書き出し
	//fwrite(&HiScore, sizeof(HiScore), 1, fp);
	fprintf_s(fout, "%d", HiScore);

	// ファイルを閉じる
	fclose(fout);
}

int HiScoreload() {
	//セーブデータ読み込み
	int hiscore = 0;
	FILE *fread;//ファイル読み込み
	errno_t error;
	error = fopen_s(&fread, "HiScore.txt", "r");
	if (error == 0)
	{
		fscanf_s(fread, "%d", &hiscore);
	}
	fclose(fread);// ファイルを閉じる

	return hiscore;
}

void howtoplay() 
{
	//char Buf[256];//GetHitKeyStateAllのやつ
	unsigned int Cr = GetColor(255, 255, 255);//白
	const int gyou = 20;

	//SetBackgroundColor(0, 0, 0);
	//GetHitKeyStateAll(Buf);

	std::string str[gyou] =
	{
		"       --遊び方--",
		"プレイヤーは点Pです。",
		"彼は右、真ん中、左に動けます。",
		"ライフゲームにかすると得点です。",
		"上下左右は20点、ななめは10点、一つ進むと1点です。",
		"一度動くたびにライフゲームが1ターン進み1マス落ちてきます。",
		"ライフゲームに飲み込まれるか移動を阻まれると死んでしまいます。",
		"ライフゲームの動きを予測しながら進んで行ってください。",
		"",
		"ライフゲームは下の様な法則で動きます。",
		"1.そのマスが緑の場合、",
		"　周囲8マスのうち2マスか3マスが緑ならそのマスは緑のまま（「生存」）",
		"2.そのマスが黒の場合、",
		"　周囲8マスのうち3マスが緑ならそのマスも緑にする（「誕生」）",
		"3.それ以外の場合、",
		"　そのマスは何色でも（黒でも緑でも）黒にしておく",
		"　（元が緑だった場合は、「死亡」）",
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
	char Buf[256];//GetHitKeyStateAllのやつ
	int key_count = 0;

	//初期配置
	life_make();

	const int masu = 16;
	bool turn = false;//ターンが1のとき機械側の操作
	int p_x = map_size_x / 2;//プレイヤーの位置
	int p_y = map_size_y - 1;
	const int turn_time = 10;//機械側のターンは何秒フレームか
	const int idou_s = turn_time + 10;//長押しで10フレームに一回進む
	int turn_count = 0;//機械側のターンを数える

	int down_count = 0;//life_downを何回実行したか 何ターン目か

	int point = 0;// 最終的な得点は point + down_count
	int point_temp = 0;//現在のターンで新たにとった得点

	int hiscore = HiScoreload();//現在のハイスコア

	unsigned int Color_b = GetColor(0, 0, 0);//黒
	unsigned int Color_w = GetColor(255, 255, 255);//白
	unsigned int Color_g = GetColor(0, 255, 0);//緑
	unsigned int Color_p = GetColor(240, 240, 240);//点Pの色
	unsigned int Color_waku = GetColor(255 / 4, 255 / 4, 255 / 4);//枠の色


	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		SetBackgroundColor(0, 0, 0);
		GetHitKeyStateAll(Buf);

		//枠表示
		DrawBox(masu/2, masu / 2, map_size_x*masu- masu , (map_size_y+1)*masu , Color_waku, TRUE);//外枠
		//DrawBox(masu - 1, masu - 1, (map_size_x-1)*masu + 1, map_size_y*masu + 1, Color_b, TRUE);//内枠（塗りつぶし）
		//DrawBox(masu - 1, (map_size_y - 11)*masu + 1, masu * 2 - 2, map_size_y *masu + 1, Color_waku, TRUE);
		//DrawBox(masu*(map_size_x - 1) + 1, (map_size_y - 11)*masu + 1, masu *(map_size_x)+1, map_size_y *masu + 1, Color_waku, TRUE);
		//DrawBox(masu / 2, (map_size_y - 11)*masu + masu / 2, masu * 2-masu/2, (map_size_y+1) *masu + 1, Color_b, TRUE);
		//DrawBox(masu*(map_size_x-1)+masu/2,  (map_size_y - 11)*masu + masu / 2, masu *(map_size_x+1), (map_size_y + 1) *masu + 1, Color_b, TRUE);
		

		//Ｐ表示
		//DrawGraph(p_x*masu, p_y*masu, ziki, FALSE);
		DrawCircle(p_x*masu+masu/2, p_y*masu + masu / 2,masu*2/16, Color_w, TRUE);
		//life表示
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

		//得点表示
		DrawFormatString((map_size_x + 1) * 16, (1) * 16, Color_w, "High score %d", hiscore);
		DrawFormatString((map_size_x + 1) * 16, (map_size_y - 3) * 16, Color_w, "POINT 前のターン+追加得点");
		DrawFormatString((map_size_x + 1) * 16, (map_size_y - 2) * 16, Color_w, "POINT: %d + %d", point + down_count - point_temp - !(down_count == 0), point_temp + 1);
		DrawFormatString((map_size_x + 1) * 16, (map_size_y - 1) * 16, Color_w, "      =%d", point + down_count);

		if (Buf[KEY_INPUT_ESCAPE] == 1) {
			break;
		}

		//移動先あるかどうか
		bool lgo = false, rgo = false, ugo = false;//目的地に到達可能ならtrue
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
		//矢印のやーつ
		if (lgo) {
			DrawFormatString((p_x - 1) * 16, yazirusi_y, Color_w, "←", point + down_count);
			DrawBox((p_x - 1) * 16, yazirusi_y, (p_x) * 16, yazirusi_y+masu, Color_w, FALSE);
		}
		if (ugo) {
			DrawFormatString((p_x) * 16, yazirusi_y, Color_w, "↑", point + down_count);
			DrawBox((p_x) * 16, yazirusi_y, (p_x + 1) * 16, yazirusi_y + masu, Color_w, FALSE);
		}
		if (rgo) {
			DrawFormatString((p_x + 1) * 16, yazirusi_y, Color_w, "→", point + down_count);
			DrawBox((p_x + 1) * 16, yazirusi_y, (p_x + 2) * 16, yazirusi_y + masu, Color_w, FALSE);
		}


		//移動右と左と留まる  ターンが進むトリガー
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

		if (turn == true) //機械側の操作
		{
			turn_count++;
			if (turn_count == turn_time*2/4)
			{
				life_update();
			}
			if (turn_count == turn_time*3/4)
			{
				if (sibouhanntei(p_x, p_y))break;//すなわち死

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
				if (!lgo && !rgo && !ugo)//移動先なければ
				{
					break;//すなわち死
				}
				turn = false;
				turn_count = 0;
			}
		}

		if ((CheckHitKeyAll() == false) && (turn != 1/*キー入力の邪魔*/)) {
			key_count = 0;
		}
	}

	//unsigned int Color;
	//Color = GetColor(0, 0, 0);
	DrawFormatString(p_x * 16, p_y * 16, Color_w, "死");


	if (hiscore < (point + down_count)) {//ハイスコア更新
		HiScoreSave(point + down_count);
	}

	WaitKey();
	life_clear();//マップ初期化
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	ChangeWindowMode(TRUE); // ウィンドウモードに設定
	const int Windowsize_x = 600;
	const int Windowsize_y = 400;
	SetGraphMode(Windowsize_x, Windowsize_y, 32);
	SetMainWindowText("life game game");
	//SetWindowSizeChangeEnableFlag(TRUE);
	DxLib_Init();   // DXライブラリ初期化処理
	char Buf[256];//GetHitKeyStateAllのやつ
	int key_count = 0;

	const int N = 3;//洗濯しの個数
	int selected = 0;//洗濯されているのは何番目か
	const int select_s = 6;//洗濯の変わる速さ
	unsigned int Color[2] = { GetColor(255,255,255), GetColor(100, 100, 100) };//洗濯されている奴が0　　
	bool c[N] = { 0,1,1 };//洗濯されている奴が0
	std::string select_str[N] = { "Game start" , "How to play", "Exit" };
	unsigned int Color_w =  GetColor(255, 255, 255);//黒色

	//エンターの入力を二回連続では受け付けないための変数　ゲームで死した後エンター押すとまた始まっちゃう
	bool flag = false;
	int hiscore = HiScoreload();//現在のハイスコア

    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		SetBackgroundColor(0, 0, 0);
		GetHitKeyStateAll(Buf);
		DrawFormatString((map_size_x + 1) * 16, 1 * 16, Color_w, "High score %d",hiscore);

		DrawFormatString(Windowsize_x / 9 - 20, Windowsize_y / 2 + 120, Color_w, "◆");
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
	DxLib_End();   // DXライブラリ終了処理
	return 0;
}