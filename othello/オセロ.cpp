#include <iostream>
#include <Windows.h>
using namespace std;

//色の値
const int COL_BLACK = 0x00;
const int COL_DARK_BLUE = 0x01;
const int COL_DARK_GREEN = 0x02;
const int COL_DARK_CYAN = 0x03;
const int COL_DARK_RED = 0x04;
const int COL_DARK_VIOLET = 0x05;
const int COL_DARK_YELLOW = 0x06;
const int COL_GRAY = 0x07;
const int COL_LIGHT_GRAY = 0x08;
const int COL_BLUE = 0x09;
const int COL_GREEN = 0x0a;
const int COL_CYAN = 0x0b;
const int COL_RED = 0x0c;
const int COL_VIOLET = 0x0d;
const int COL_YELLOW = 0x0e;
const int COL_WHITE = 0x0f;
const int COL_INTENSITY = 0x08;	
const int COL_RED_MASK = 0x04;
const int COL_GREEN_MASK = 0x02;
const int COL_BLUE_MASK = 0x01;

//	文字色指定 for Windows Console
void setColor(int col){
	HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD attr = 0;
	if (col & COL_INTENSITY)
		attr |= FOREGROUND_INTENSITY;
	if (col & COL_RED_MASK)
		attr |= FOREGROUND_RED;
	if (col & COL_GREEN_MASK)
		attr |= FOREGROUND_GREEN;
	if (col & COL_BLUE_MASK)
		attr |= FOREGROUND_BLUE;
	SetConsoleTextAttribute(hCons, attr);
}
void setColor(int fg, int bg){
	HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD attr = 0;
	if (fg & COL_INTENSITY)
		attr |= FOREGROUND_INTENSITY;
	if (fg & COL_RED_MASK)
		attr |= FOREGROUND_RED;
	if (fg & COL_GREEN_MASK)
		attr |= FOREGROUND_GREEN;
	if (fg & COL_BLUE_MASK)
		attr |= FOREGROUND_BLUE;

	if (bg & COL_INTENSITY)
		attr |= BACKGROUND_INTENSITY;
	if (bg & COL_RED_MASK)
		attr |= BACKGROUND_RED;
	if (bg & COL_GREEN_MASK)
		attr |= BACKGROUND_GREEN;
	if (bg & COL_BLUE_MASK)
		attr |= BACKGROUND_BLUE;
	SetConsoleTextAttribute(hCons, attr);
}

//定義
const int field_size = 8;//フィールドの大きさ
const int first_black = 1;//先攻(黒)
const int second_white = 2;//後攻(白)
const int vec_amount = 8;//8方向のベクトル
const int vec_x_scan[] = { 0, 1, 1, 1, 0, -1, -1, -1 };//方向ベクトルの定義
const int vec_y_scan[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

int field_trout[field_size + 1][field_size + 1];//フィールド
int field_trout_buffer[field_size][field_size];//フィールドのバッファ
int width_input = 0;//横の入力
int heigt_input = 0;//縦の入力
int first_or_second = first_black;//順番
int errer_piece = 0;//エラー判定
int winner = 0;//勝利判定

//初期のフィールド作成
void FIELD_INITIAL(void) {
	//縦
	for (int i = 0; field_size > i; ++i) {
		//横
		for (int t = 0; field_size > t; ++t) {
			//その他のマスを0に指定
			field_trout[i][t] = 0;

			//初期のマスに白→１, 黒→２
			if (i == 3 && t == 4 || i == 4 && t == 3) {
				field_trout[i][t] = 1;
			}
			if (i == 3 && t == 3 || i == 4 && t == 4) {
				field_trout[i][t] = 2;
			}
		}
	}
}

//先攻後攻の設定
void DICEDE_FIRST_OR_SECOND(void) {
	setColor(COL_BLACK, COL_WHITE);
	if (first_or_second == first_black) {
		cout << "先攻(黒)" << endl;
	}
	if (first_or_second == second_white) {
		cout << "後攻(白)" << endl;
	}
}

//横の入力
void INPUT_OPERATION_WIDTH(void) {
	int check = 0;
	//無限ループ
	for (;;) {
		//最初のループで実行
		if (check == 0) {
			setColor(COL_WHITE);
			cout << "【横の列の";
		}
		//2回目以降のループではこちら
		else {
			cout << "【";
			setColor(COL_VIOLET);
			cout << "1~8の範囲内";
			setColor(COL_WHITE);
			cout << "で横の列の";
		}
		setColor(COL_RED);
		cout << "赤色の数字";
		setColor(COL_WHITE);
		cout << "の番号を入力してください】" << endl << ">";
		cin >> width_input;

		//判定
		if (width_input >= 1 && width_input <= 8)
			break;

		check = 1;
		width_input = 0;
	}

}

//縦の入力
void INPUT_OPERATION_HEIGHT(void) {
	int check = 0;
	//無限ループ
	for (;;) {
		//最初のループで実行
		if (check == 0) {
			setColor(COL_WHITE);
			cout << "【縦の列の";
		}
		//二回目以降のループではこちら
		else {
			cout << "【";
			setColor(COL_VIOLET);
			cout << "1~8の範囲内";
			setColor(COL_WHITE);
			cout << "で縦の列の";
		}
		setColor(COL_BLUE);
		cout << "青色の数字";
		setColor(COL_WHITE);
		cout << "の番号を入力してください】" << endl << ">";
		cin >> heigt_input;

		//判定
		if (heigt_input >= 1 && heigt_input <= 8) 
			break;

		check = 1;
		heigt_input = 0;
	}
}

//バッファの色の変更
void COLOR_CHANGE(void) {
	//縦のループ
	for (int i = 0; field_size > i; ++i) {
		//選択した縦を着色
		if (width_input != 0) 
			field_trout_buffer[i][width_input - 1] = 1;
		//横のループ
		for (int t = 0; field_size > t; ++t) {
			//選択した縦かつ選択した横のマスを着色
			if (heigt_input != 0 && field_trout_buffer[heigt_input - 1][t] == 1) 
				field_trout_buffer[heigt_input - 1][t] = 2;	
		}
		//その他の余分なマスの着色を消去
		if (heigt_input != 0 && field_trout_buffer[i][width_input - 1] != 2)
			field_trout_buffer[i][width_input - 1] = 0;
	}
}

//出力
void OUTPUT(void) {
		//上段の赤の番号を出力し改行
	setColor(COL_RED, COL_CYAN);
	cout << "　";
	cout << "１２３４５６７８";
	cout << endl;

	//縦
	for (int i = 0; field_size  > i; ++i) {
		//縦の青の番号の出力
		setColor(COL_BLUE, COL_CYAN);
		cout << i + 1 << " ";

		//横
		for (int t = 0; field_size > t; ++t) {
			switch (field_trout[i][t]){
				//なにもないところ
			case 0:
				if (field_trout_buffer[i][t] == 1) {
					setColor(COL_LIGHT_GRAY, COL_YELLOW);
					cout << "・";
				}
				else if (field_trout_buffer[i][t] == 2) {
					setColor(COL_LIGHT_GRAY, COL_GREEN);
					cout << "●";
				}
				else {
					setColor(COL_LIGHT_GRAY, COL_BLACK);
					cout << "●";
				}
				break;
				//黒
			case 1:
				if (field_trout_buffer[i][t] == 1) {
					setColor(COL_BLACK, COL_YELLOW);
					cout << "●";
				}
				else if (field_trout_buffer[i][t] == 2) {
					setColor(COL_BLACK, COL_GREEN);
					cout << "●";
				}
				else {
					setColor(COL_WHITE, COL_BLACK);
					cout << "○";
				}
				break;
				//白
			case 2:
				if (field_trout_buffer[i][t] == 1) {
					setColor(COL_WHITE, COL_YELLOW);
					cout << "●";
				}
				else if (field_trout_buffer[i][t] == 2) {
					setColor(COL_WHITE, COL_GREEN);
					cout << "●";
				}
				else {
					setColor(COL_WHITE, COL_BLACK);
					cout << "●";
				}
				break;
			}
			
		}
		setColor(COL_WHITE);
		cout << "  ";
		cout << endl;
	}
	setColor(COL_WHITE);
	cout << "                    " << endl;
}

//フィールドバッファをクリア
void MOVE_BUFFER(void){
	for (int i = 0; field_size > i; ++i) {
		for (int t = 0; field_size > t; ++t) {
			field_trout_buffer[i][t] = 0;

		}
	}
}

//裏返し
void TURN_OVER(int vec_count, int multiple) {
	for (int p = 0; multiple > p; ++p) {
		if (first_or_second == first_black) {
			field_trout[heigt_input - 1 + (vec_x_scan[vec_count] * p)][width_input - 1 + vec_y_scan[vec_count] * p] = first_black;
		}
		if (first_or_second == second_white) {
			field_trout[heigt_input - 1 + (vec_x_scan[vec_count] * p)][width_input - 1 + vec_y_scan[vec_count] * p] = second_white;
		}
	}
}

//駒の配置判定
void PLACEMENT_JUDGMENT(void) {
	int count_flag = 0;

	//そのマスがもう埋まっていた場合
	if (field_trout[heigt_input - 1][width_input - 1] != 0) 
		errer_piece = 1;
	else {
		//隣接するマスに駒があるのをカウント(黒の場合)
		if (first_or_second == first_black) {
			//8方向のベクトルを走査しそこに白の駒はあるか
			for (int i = 0; vec_amount > i; ++i) {
				if (field_trout[heigt_input - 1 + vec_x_scan[i]][width_input - 1 + vec_y_scan[i]] == second_white) {
					//その白の駒から対角線上に黒のコマはあるか
					for (int t = 2; field_size > t; ++t) {
						if (field_trout[heigt_input - 1 + (vec_x_scan[i] * t)][width_input - 1 + (vec_y_scan[i] * t)] == first_black) {
							count_flag++;
							TURN_OVER(i, t);
							break;
						}
						if (field_trout[heigt_input - 1 + (vec_x_scan[i] * t)][width_input - 1 + (vec_y_scan[i] * t)] == 0)
							break;
					}

				}
			}
			if (count_flag == 0)
				errer_piece = 1;
			count_flag = 0;
		}
		//隣接するマスに駒があるのをカウント(白の場合)
		if (first_or_second == second_white) {
			//8方向のベクトルを走査しそこに黒の駒はあるか
			for (int i = 0; vec_amount > i; ++i) {
				if (field_trout[heigt_input - 1 + vec_x_scan[i]][width_input - 1 + vec_y_scan[i]] == first_black) {
					for (int t = 2; field_size > t; ++t) {
						if (field_trout[heigt_input - 1 + (vec_x_scan[i] * t)][width_input - 1 + (vec_y_scan[i] * t)] == second_white) {
							count_flag++;
							TURN_OVER(i, t);
							break;
						}
						if (field_trout[heigt_input - 1 + (vec_x_scan[i] * t)][width_input - 1 + (vec_y_scan[i] * t)] == 0)
							break;
					}
				}
			}
			if (count_flag == 0)
				errer_piece = 1;
			count_flag = 0;
		}
	}
	
}

//エラーの出力
void ERRER_OUTPUT(void) {
	setColor(COL_VIOLET, COL_WHITE);
	if (errer_piece == 1) {
		cout << "【ERRER】駒がすでに置いてあるか、置ける場所ではありません。もう一度入力してください。" << endl;
	}
	else if (errer_piece == 2) {
		if (first_or_second == first_black)
			cout << "白の駒はおけない為スキップしました。" << endl;
		else
			cout << "黒の駒はおけない為スキップしました。" << endl;
	}
}

//スキップ判定
void SKIP_JUDGMENT(void) {
	int errer_buffer = 0;
	//空白のコマからベクトルを走査し置けるかどうかを確かめる
	for (int i = 0; field_size > i; ++i) {
		for (int t = 0; field_size > t; ++t) {
			//何も埋まっていないマス
			if (field_trout[i][t] == 0) {
				//黒の場合
				if (first_or_second == first_black) {
					//8方向のベクトルを走査しそこに白の駒はあるか
					for (int p = 0; vec_amount > p; ++p) {
						if (field_trout[i + vec_x_scan[p]][t + vec_y_scan[p]] == second_white) {
							//その白の駒から対角線上に黒のコマはあるか
							for (int q = 2; field_size > q; ++q) {
								if (field_trout[i + (vec_x_scan[p] * q)][t + (vec_y_scan[p] * q)] == first_black)
									errer_buffer++;
							}
						}
					}
				}
				//白の場合
				else {
					//8方向のベクトルを走査しそこに黒の駒はあるか
					for (int p = 0; vec_amount > p; ++p) {
						if (field_trout[i + vec_x_scan[p]][t + vec_y_scan[p]] == first_black) {
							//その白の駒から対角線上に黒のコマはあるか
							for (int q = 2; field_size > q; ++q) {
								if (field_trout[i + (vec_x_scan[p] * q)][t + (vec_y_scan[p] * q)] == second_white)
									errer_buffer++;
							}
						}
					}
				}
			}
		}
	}
	if (errer_buffer == 0) {
		errer_piece = 2;
		//先攻後攻を戻す
		switch (first_or_second) {
		case first_black:
			first_or_second = second_white;
			break;
		case second_white:
			first_or_second = first_black;
			break;
		}
	}
}

//勝利判定
int VICTORY_JUDGMENT(void) {
	int blank = 0, black_size = 0, white_size = 0, winner = 0;

	for (int i = 0; field_size > i; ++i) {
		for (int t = 0; field_size > t; ++t) {
			if (field_trout[i][t] == 0)
				blank++;
			if (field_trout[i][t] == 1)
				black_size++;
			if (field_trout[i][t] == 2)
				white_size++;
		}
	}
	if (blank == 0) {
		if (black_size > white_size)
			winner = 1;
		else if (white_size > black_size)
			winner = 2;
		else
			winner = 3;
	}
	return winner;
}

//勝利画面
void VICTORY_SCREEN(void) {
	int color_sort = 0;
	system("cls");
	//無限ループさせ文字の色を更新していく
	for (;;) {
		switch (color_sort){
		case 0:
			setColor(COL_WHITE);
			break;
		case 1:
			setColor(COL_CYAN);
			break;
		case 2:
			setColor(COL_YELLOW);
			break;
		case 3:
			setColor(COL_VIOLET);
			break;
		case 4:
			setColor(COL_GREEN);
			break;
		case 5:
			setColor(COL_RED);
			break;
		case 6:
			setColor(COL_BLUE);
			break;
		}
		cout << "V";
		switch (color_sort) {
		case 0:
			setColor(COL_CYAN);
			break;
		case 1:
			setColor(COL_YELLOW);
			break;
		case 2:
			setColor(COL_VIOLET);
			break;
		case 3:
			setColor(COL_GREEN);
			break;
		case 4:
			setColor(COL_RED);
			break;
		case 5:
			setColor(COL_BLUE);
			break;
		case 6:
			setColor(COL_WHITE);
			break;
		}
		cout << "I";
		switch (color_sort) {
		case 0:
			setColor(COL_YELLOW);
			break;
		case 1:
			setColor(COL_VIOLET);
			break;
		case 2:
			setColor(COL_GREEN);
			break;
		case 3:
			setColor(COL_RED);
			break;
		case 4:
			setColor(COL_BLUE);
			break;
		case 5:
			setColor(COL_WHITE);
			break;
		case 6:
			setColor(COL_CYAN);
			break;
		}
		cout << "C";
		switch (color_sort) {
		case 0:
			setColor(COL_VIOLET);
			break;
		case 1:
			setColor(COL_GREEN);
			break;
		case 2:
			setColor(COL_RED);
			break;
		case 3:
			setColor(COL_BLUE);
			break;
		case 4:
			setColor(COL_WHITE);
			break;
		case 5:
			setColor(COL_CYAN);
			break;
		case 6:
			setColor(COL_YELLOW);
			break;
		}
		cout << "T";
		switch (color_sort) {
		case 0:
			setColor(COL_GREEN);
			break;
		case 1:
			setColor(COL_RED);
			break;
		case 2:
			setColor(COL_BLUE);
			break;
		case 3:
			setColor(COL_WHITE);
			break;
		case 4:
			setColor(COL_CYAN);
			break;
		case 5:
			setColor(COL_YELLOW);
			break;
		case 6:
			setColor(COL_VIOLET);
			break;
		}
		cout << "O";
		switch (color_sort) {
		case 0:
			setColor(COL_RED);
			break;
		case 1:
			setColor(COL_BLUE);
			break;
		case 2:
			setColor(COL_WHITE);
			break;
		case 3:
			setColor(COL_CYAN);
			break;
		case 4:
			setColor(COL_YELLOW);
			break;
		case 5:
			setColor(COL_VIOLET);
			break;
		case 6:
			setColor(COL_GREEN);
			break;
		}
		cout << "R";
		switch (color_sort) {
		case 0:
			setColor(COL_BLUE);
			break;
		case 1:
			setColor(COL_WHITE);
			break;
		case 2:
			setColor(COL_CYAN);
			break;
		case 3:
			setColor(COL_YELLOW);
			break;
		case 4:
			setColor(COL_VIOLET);
			break;
		case 5:
			setColor(COL_GREEN);
			break;
		case 6:
			setColor(COL_RED);
			break;
		}
		cout << "Y";

		setColor(COL_YELLOW, COL_DARK_BLUE);
		//2個改行
		cout << endl << endl;
		switch (winner){
		case 1:
			cout << "先攻(黒)の勝利" << endl;
			break;
		case 2:
			cout << "後攻(白)の勝利" << endl;
			break;
		case 3:
			cout << "引き分け" << endl;
			break;
		}
		//何ティックか待機
		Sleep(1000);
		//画面クリア
		system("cls");

		//color_sortを+1づつしていき6以上になったら0にする
		if (color_sort < 6)
			color_sort++;
		else
			color_sort = 0;
	}
}

//メイン
int main() {
	//エラーのやつ
#if	0
	setColor(COL_BLACK, COL_LIGHT_GRAY);
	cout << "hoge  hoge\n";
	setColor(COL_YELLOW);
	cout << "hoge  hoge\n";
#endif
	
	FIELD_INITIAL();//初期フィールドの作成
	for (;;) { //無限ループ
		
		for (;;) {
			width_input = 0;
			heigt_input = 0;

			OUTPUT();//出力
			cout << endl;
			setColor(COL_WHITE);
			cout << "---------------------------" << endl;
			DICEDE_FIRST_OR_SECOND();
			ERRER_OUTPUT();
			INPUT_OPERATION_WIDTH();
			COLOR_CHANGE();
			system("cls");

			OUTPUT();//出力
			cout << endl;
			setColor(COL_WHITE);
			cout << "---------------------------" << endl;
			DICEDE_FIRST_OR_SECOND();
			ERRER_OUTPUT();
			INPUT_OPERATION_HEIGHT();
			COLOR_CHANGE();
			system("cls");

			errer_piece = 0;
			OUTPUT();//出力
			PLACEMENT_JUDGMENT();//判定
			MOVE_BUFFER();//バッファクリア
			Sleep(1000);
			system("cls");
			if (errer_piece == 0)
				break;

		}
		//先攻後攻のチェンジ
		switch (first_or_second){
		case first_black:
			first_or_second = second_white;
			break;
		case second_white:
			first_or_second = first_black;
			break;
		default:
			break;
		}
		SKIP_JUDGMENT();
		winner = VICTORY_JUDGMENT();
		if (winner == 1 || winner == 2 || winner == 3)
			break;
		winner = 0;

	}
	VICTORY_SCREEN();

	//最後の文字を戻す
	setColor(COL_WHITE);
}
