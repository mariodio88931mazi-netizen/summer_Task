/*ジャックポット
概要：
・CPUとプレイヤーが交互に２つの六面サイコロを振り、1から9までのマスを陣取りするゲームです。

ゲームの流れ：
・プレイヤーは交互に六面サイコロを2個振る。
・出た目の数に合わせて、対応するマスを取ることができます。
　プレイヤーは以下のうち1つを選択できる:
　　①六面サイコロの目
　　②六面サイコロの目の合計
例：
　六面サイコロの目が、1と5の場合→1、5、6のどれかのマスを取る事が出来ます。
・取られているもしくは取っている場合は、取る事はできません。
・この一連の動作を交互に行ってください。
ゲームの終了：
・どちらかのプレイヤーがマスを取る事が出来なくなった時点でゲーム終了。
・もし全てのマスが取った場合、そのターンでマスを取ったプレイヤーが勝ちとなります。
*/
#include<iostream>
#include<cstdlib>
#include<ctime>

using namespace std;

//定数宣言
const int SIZE = 9;
const int CELL_MAX = 9;
const int CELL_MIN = 1;
const int DICE_MAX = 6;
const int DICE_0_DELETE = 1;
const int SAIKORO_RANDOM_CHOICE = 2;
const int RESET = 0;
const int INPUT_MAX = 2;
const int INPUT_MIN = 0;
const int CELL_INDEX_OFFSET = 1;
const int CELL_FREE = 0;
const int PLAYER_CELL = 1;
const int CPU_CELL = 2;
const int CPU_RANDOM_CHOICE = 3;


int InputCheker(int min, int max);
int diceRoll(int& saikoro1, int& saikoro2, int& total);
bool isCelltaken(const int masu[], int player);
bool CanTakeCell(const int masu[], int saikoro1, int saikoro2, int total);
void displayBoard(const int masu[], int size);
int choiceNumber(int playerChoice, int saikoro1, int saikoro2, int total, int& player);
bool isGameOver(const int masu[], int size);

int main(void)
{
	srand((unsigned)time(NULL));

	//変数宣言
	int dice1{}, dice2{}, diceTotal{};
	int playerChoice;
	int playerCell{};
	int cpuChoice{};
	int cpuCell{};
	int board[SIZE] = { RESET,RESET,RESET,RESET,RESET,RESET,RESET,RESET,RESET };
	int index{};
	int playerpoint{};
	int cpupoint{};

	cout << "================ジャックポットゲーム！=================\n" << endl;
	//ゲームループ
	while (true)
	{
		diceTotal = RESET;
		//プレイヤーのターン
		cout << "プレイヤーのターンです。" << endl;

		//マス表示
		displayBoard(board, SIZE);

		//サイコロを振る

		diceTotal = diceRoll(dice1, dice2, diceTotal);

		while (true)
		{
			//サイコロの目を表示
			cout << "プレイヤーのサイコロの目は、" << "\n＜サイコロ１＞＝" << dice1 << "\n＜サイコロ２＞＝" << dice2 << "\n＜  合計値  ＞＝" << diceTotal << "\nです。" << endl;
			cout << "どの数字を使いますか？\nサイコロ１ = 1キー:   サイコロ２ = 2キー:   合計値 = ０キー:\n~~~対応するキーを入力~~~" << endl;
			playerChoice = InputCheker(INPUT_MIN, INPUT_MAX);

			choiceNumber(playerChoice, dice1, dice2, diceTotal, playerCell);

			//マスの状態を確認
			if (!CanTakeCell(board, dice1, dice2, diceTotal))
			{
				break;
			}
			if (!isCelltaken(board, playerCell))
			{
				// プレイヤーが再度選べるようにループする
				continue;
			}
			else
			{
				break;
			}

		}

		if (!CanTakeCell(board, dice1, dice2, diceTotal))
		{
			// プレイヤーがマスを取れない場合、ゲーム終了
			cout << "プレイヤーはマスを取れません。" << endl;
			cout << "ゲーム終了です。" << endl;
			cout << "CPUの勝利です。" << endl;
			break;
		}

		board[playerCell - CELL_INDEX_OFFSET] = PLAYER_CELL;
		playerpoint++;
		if (isGameOver(board, SIZE))
		{
			cout << "全てのマスが取られました。" << endl;
			cout << "＜＜＜＜＜＜プレイヤーの勝利です！＞＞＞＞＞＞" << endl;
			break;
		}

		//CPUのターン
		cout << "cpuのターンです。" << endl;

		//マス表示
		displayBoard(board, SIZE);

		//サイコロを振る
		diceTotal = diceRoll(dice1, dice2, diceTotal);

		//サイコロの目を表示
		cout << "CPUのサイコロの目は、" << "\n＜サイコロ１＞＝" << dice1 << "\n＜サイコロ２＞＝" << dice2 << "\n＜  合計値  ＞＝" << diceTotal << "\nです。" << endl;

		while (true)
		{
			//CPUの選択をランダムに決定
			cpuChoice = rand() % CPU_RANDOM_CHOICE;
			cout << "CPUは、" << cpuChoice << "キー" << " を選択しました。" << endl;

			choiceNumber(cpuChoice, dice1, dice2, diceTotal, cpuCell);

			if (cpuCell >= CELL_MIN && cpuCell <= CELL_MAX &&
				board[cpuCell - CELL_INDEX_OFFSET] == CELL_FREE)
			{
				break;
			}

			cout << "そのマスは取れません。CPUがもう一度選びます。" << endl;

			if (!CanTakeCell(board, dice1, dice2, diceTotal))
			{
				break;
			}
		}

		if (!CanTakeCell(board, dice1, dice2, diceTotal))
		{
			// CPUがマスを取れない場合、ゲーム終了
			cout << "CPUはマスを取れません。" << endl;
			cout << "ゲーム終了です。" << endl;
			cout << "プレイヤーの勝利です。" << endl;
			break;
		}

		board[cpuCell - CELL_INDEX_OFFSET] = CPU_CELL;
		cpupoint++;
		if (isGameOver(board, SIZE))
		{
			cout << "全てのマスが取られました。" << endl;
			cout << "＜＜＜＜＜＜CPUの勝利です！＞＞＞＞＞＞" << endl;
			break;
		}

		cout << "CPUは、" << cpuCell << "のマスを取りました。" << endl;
	}
}

void displayBoard(const int masu[], int size)
{
	cout << "============================================" << endl;
	cout << "現在のマスの状態は、" << endl;
	cout << "1 2 3 4 5 6 7 8 9" << endl;
	for (int i = 0; i < size; ++i)
	{
		if (masu[i] == CELL_FREE)
		{
			cout << "□ ";
		}
		else if (masu[i] == PLAYER_CELL)
		{
			cout << "■ ";
		}
		else
		{
			cout << "◆ ";
		}
	}
	cout << "\n============================================" << endl;
	cout << endl;
}

int diceRoll(int& saikoro1, int& saikoro2, int& total)
{
	//サイコロを振る
	saikoro1 = rand() % DICE_MAX + DICE_0_DELETE;
	saikoro2 = rand() % DICE_MAX + DICE_0_DELETE;
	total = saikoro1 + saikoro2;
	return total;
}

int choiceNumber(int playerChoice, int saikoro1, int saikoro2, int total, int& player)
{
	int randomDice = rand() % SAIKORO_RANDOM_CHOICE; //0, 1のいずれかをランダムに選択
	switch (playerChoice)
	{
	case 0:
		player = total;
		if (player >= 10)
		{
			cout << "\n*サイコロの目が10以上のため、サイコロ１かサイコロ２を選択します。*\n" << endl;

			//ランダムにサイコロ1かサイコロ2を選択
			player = randomDice == 0 ? saikoro1 : saikoro2;
			if (randomDice == 0)
			{
				cout << "サイコロ１にしました。" << endl;
			}
			else
			{
				cout << "サイコロ２にしました。" << endl;
			}
		}
		break;
	case 1:
		player = saikoro1;
		break;
	case 2:
		player = saikoro2;
		break;
	}
	return player;
}

bool isCelltaken(const int masu[], int player)
{
	if (masu[player - CELL_INDEX_OFFSET] == PLAYER_CELL)
	{
		cout << "\n*そのマスはプレイヤーに既に取られています。別のマスを選択してください。*\n" << endl;
		return false;
	}
	if (masu[player - CELL_INDEX_OFFSET] == CPU_CELL)
	{
		cout << "\n*そのマスはCPUが既に取っています。別のマスを選択してください。*\n" << endl;
		return false;
	}

	return true;
}

bool CanTakeCell(const int masu[], int saikoro1, int saikoro2, int total)
{
	// サイコロ1の目が空いているか
	if (masu[saikoro1 - CELL_INDEX_OFFSET] == CELL_FREE)
	{
		return true;
	}

	// サイコロ2の目が空いているか
	if (masu[saikoro2 - CELL_INDEX_OFFSET] == CELL_FREE)
	{
		return true;
	}

	// 合計値が1～9の範囲なら、合計値のマスが空いているか
	if (total >= CELL_MIN && total <= CELL_MAX)
	{
		if (masu[total - CELL_INDEX_OFFSET] == CELL_FREE)
		{
			return true;
		}
	}

	// どのマスも取れない
	return false;
}

int InputCheker(int min, int max)
{
	int player;
	while (true)
	{
		cin >> player;
		if (player < min || player > max)
		{
			cout << "入力が不正です。もう一度入力してください。" << endl;
		}
		else
		{
			break;
		}
	}
	return player;
}

bool isGameOver(const int masu[], int size)
{
	for (int i = 0; i < size; ++i)
	{
		if (masu[i] == CELL_FREE)
		{
			return false; // まだ空きマスがある
		}
	}
	return true; // 全てのマスが取られた
}
