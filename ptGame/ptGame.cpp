#include "stdafx.h"
//#pragma warning( disable : 4996)

#include <iostream>

#include <limits>

#include <vector>

#include <queue>

#include <stack>

#include <set>

#include <map>

#include<ctime>

#include<string>

using namespace std;



//����һ��4*4�Ĳ���        

#define SQUARES 5

//������ת���������Ļ����������ڿ�����չ

#define BASE 25                  

//DFS������������

#define DEPTHBOUND 50              

//·������󳤶ȣ����費�ᳬ��50��Ϊ�����������װ���£�

#define STEPSBOUND 300                

int STEPSBOUND1 = 12;

#define STEPSBOUND2 3

#define MOVE_LEFT (-1)

#define MOVE_RIGHT 1

#define MOVE_UP (-SQUARES)

#define MOVE_DOWN SQUARES

#define MOVE_NONE 0



//����һ�������پ����Ԥ���       

int manhattan[SQUARES * SQUARES][SQUARES * SQUARES];

//�������ƶ�

int move1[SQUARES];



//һ������������Ϣ�����ݽṹ

struct node

{

	//����vector����װ�ǰ��״̬

	vector <int> state;

	//װ��һ�����Ž⣬�������ߵ�����

	int moves[STEPSBOUND];

	//��ǰ�����            

	int depth;

	//��ǰ�Ľڵ�Ĺ���ֵ����������ʽ������

	int score;

	//�ո��λ��

	int blank;

};



//���ȶ��еıȽϺ���,��ֵ��С�������ȶ��е�ǰ��(���������)

bool operator<(node x, node y)

{

	return x.score > y.score;

}



//�����ֵ

int absi(int x)

{

	return x >= 0 ? x : (-x);

}



//�жϸ��������Ƿ�ɽ�,���������Ĳ��ԣ���������Ե���ż��ԭ��

bool solvable(vector <int> tiles)

{

	int sum = 0, row;

	for (int i = 0; i < tiles.size(); i++)

	{

		int tile = tiles[i];

		if (tile == 0)

		{

			row = (i / SQUARES + 1);

			continue;

		}

		for (int m = i; m < tiles.size(); m++)

		if (tiles[m] < tile && tiles[m] != 0)

			sum++;

	}

	//return !((sum + row) % 2);
	return true;
}



/*

�õ���ǰ����ĺ���߷���MOVE_LEFT = �����ƶ��ջ��飬MOVE_RIGHT = �����ƶ��ջ��飬

MOVE_UP = �����ƶ��ջ��飬MOVE_DOWN = �����ƶ��ջ���

*/



void valid_moves(node &current)

{

	//��ȡ����߷�������ȥ�˻ص���״̬����һ�����߷�

	int last_move = MOVE_NONE;

	//�������ȷ��֮�󣬾Ϳ���ȷ�����һ����������

	if (current.depth)

		last_move = current.moves[current.depth - 1];

	memset(move1, MOVE_NONE, sizeof(move1));

	//�����ĸ��������ĸ�������ƶ����з�Χ�ĺϷ�����Լ

	if (current.blank % SQUARES > 0 && last_move != MOVE_RIGHT)

		move1[0] = MOVE_LEFT;

	if (current.blank % SQUARES < (SQUARES - 1) && last_move != MOVE_LEFT)

		move1[1] = MOVE_RIGHT;

	if (current.blank / SQUARES > 0 && last_move != MOVE_DOWN)

		move1[2] = MOVE_UP;

	if (current.blank / SQUARES < (SQUARES - 1) && last_move != MOVE_UP)

		move1[3] = MOVE_DOWN;

}



//������״̬ת��Ϊһ������(��֤ÿһ��״̬����һ��Ψһ������ֵ)

unsigned long long key(vector <int> &tiles)

{

	unsigned long long key = 0;

	for (int i = 0; i < tiles.size(); i++)

		key = key * BASE + tiles[i];

	return key;

}



//�Ӿ��� current ִ�� move ��ָ�����߷�

node execute(node &current, int move)

{

	node successor;

	//�߷������趨

	memcpy(successor.moves, current.moves, sizeof(current.moves));

	successor.depth = current.depth + 1;

	successor.moves[current.depth] = move;

	//����״̬�趨,�� move ָ�������ƶ��������ջ���λ��(��������ǽ������������λ��)

	successor.state = current.state;

	successor.blank = current.blank + move;

	successor.state[current.blank] = successor.state[successor.blank];

	successor.state[successor.blank] = 0;

	return successor;

}
int blanked = 24;

void execute1(int current[], int move)

{

	//����״̬�趨,�� move ָ�������ƶ��������ջ���λ��(��������ǽ������������λ��)

	

	int blank = blanked + move;

	current[blanked] = current[blank];

	cout << blank << endl;

	current[blank] = 0;
	blanked = blank;

	for (int i = 0; i < 25; i++){
		if (i % 5 == 0) cout << endl;
		cout << current[i] << " ";
		
	}

	cout << endl;
}


/*

���� h*��n�� ���㷨�зǳ��ؼ����������Ǹ߶��ػ��ģ���������Ĳ�ͬ����ͬ��������Ҫ�ҵ�һ������

�� h*��n�� �����ǱȽ����ѵģ�������ʹ�õ���ÿ�����鵽��Ŀ��λ�õ������پ���֮�ͣ������پ�����

��״̬Ҫ�ﵽĿ��״̬������Ҫ�ƶ��Ĳ�������g*��n�� Ϊ�����״̬����ȣ����������������������

���� f*��n�� = g*��n�� + 4 / 3 * h*��n����h*��n�� Ϊ��ǰ״̬��Ŀ��״̬�������پ��룬���

�Կ��Ǽ�����������Ծ��롣������ʵ����һ�£�Ч�ʱȵ��������پ���Ҫ�ߣ����������پ�������ʵ�ϵ

���ķ����͡��ɲο���

[Bernard Bauer��The Manhattan Pair Distance Heuristic for the 15-Puzzle��1994]

*/



//��������ۺ����ۺϵؿ��ǵ��������پ������������ȣ����ԱȽ�����

int score(vector <int> &state, int depth)

{

	int hn = 0;

	for (int i = 0; i < state.size(); i++)

	if (state[i] > 0)

		hn += manhattan[state[i] - 1][i];

	return (depth + STEPSBOUND1* hn / STEPSBOUND2);

}



//�ж��Ƿ��ѴﵽĿ��״̬��
int cc = 0;
bool solved(vector < int > &state)

{
	string t;
	//�������һ��Ԫ���Ƿ���0,���ȿ������һ��Ԫ�أ���ʵ����һ�ּ�֦������

	if (state[SQUARES * SQUARES - 1] != 0)
		return false;
	//for (int i = 0; i < SQUARES * SQUARES - 1; i++) cout << state[i]<<" ";
	//cc++;
	//cout <<cc << endl;
	for (int i = 0; i < SQUARES * SQUARES - 1; i++)

	if (state[i] != (i + 1))

		return false;

	return true;

}



//�ҵ�����״̬�Ŀջ���λ��

int find_blank(vector < int > &state)

{

	for (int i = 0; i < SQUARES * SQUARES; i++)

	if (state[i] == 0)

		return i;

}

/*

[IDA* ����]

����������ڴ�ռ�÷���ռ���ƣ���������û�м�֦�����������ռ�޴�A* ������Ȼ��֦���������ڴ�

���˲�����ÿ���ڵ㣬�ڴ����Ľϴ�IDA* ������������ߵ����ơ�IDA* ʵ���Ͼ������������������

�㷨��ʹ������ʽ������������Ƚ�������

*/


clock_t START;

bool solve_puzzle_by_iterative_deepening_a_star(vector < int > tiles, int directions[])

{

	node copy;

	copy.state = tiles;

	copy.depth = 0;

	copy.blank = find_blank(tiles);

	memset(copy.moves, MOVE_NONE, sizeof(copy.moves));



	//��⵱ǰ�����Ƿ�Ϊ�ѽ��״̬

	if (solved(copy.state))

	{

		memcpy(directions, copy.moves, sizeof(copy.moves));

		return true;

	}



	//�趨��ʼ�������Ϊ��ʼ״̬������

	int depth_limit = 0, min_depth = score(copy.state, 0);

	while (true)

	{

		//��ȡ�����������

		if (depth_limit < min_depth)

			depth_limit = min_depth;

		else

			//��ʼ������һ��

			depth_limit++;



		//��ʼ�µ�����������أ����Ϊ depth_limit

		stack < node > open;

		open.push(copy);

		while (!open.empty())

		{

			node current = open.top();

			open.pop();

			//��ȡ�þ���ĺ���߷�������߷�������뿪�ż���

			valid_moves(current);

			for (int i = 0; i < SQUARES; i++)

			{

				if (move1[i] == MOVE_NONE)

					continue;

				//�ڵ�ǰ������ִ���߷�

				node successor = execute(current, move1[i]);

				//��¼�����ǰһ���߷�������ҵ��⣬��ô�����˳�������Ļ������Ƶ�����ڽ�����뿪�ż�   

				if (solved(successor.state))

				{

					memcpy(directions, successor.moves, sizeof(successor.moves));

					return true;

				}

				//���㵱ǰ�ڵ�����֣���С�����ƣ�����ջ�У������ҳ�������Сֵ

				successor.score = score(successor.state, successor.depth);

				if (successor.score < depth_limit)

					open.push(successor);

				else

				{

					if (successor.score < min_depth)

						min_depth = successor.score;

				}

			}
			if (clock() - START>2000) {
				cout << clock() - START << "  " << STEPSBOUND1<< endl;
				return false;
			}
		}
		
	}

	return false;

}



bool solve_puzzle(int &current,vector <int> tiles)

{

	//���������BFS,DFS,A*��IDA*һ�����ַ���

	int moves[STEPSBOUND];

	// �������������

	// solve_puzzle_by_depth_first_search(tiles, moves);

	// �������������

	// solve_puzzle_by_breadth_first_search(tiles, moves);

	// A* �������ⳤ���� 30 - 50 ��֮��ľ���ƽ����Ҫ 7 s��UVa RT 1.004 s��

	// solve_puzzle_by_a_star(tiles, moves);

	// IDA* �������ⳤ���� 30 - 50 ��֮��ľ���ƽ����Ҫ 1.5 s��UVa RT 0.320 s��

	if (!solve_puzzle_by_iterative_deepening_a_star(tiles, moves)) return false;

	// ����߷����衣

	for (int i = 0; i < STEPSBOUND; i++)

	{
		
		//��һ������ǳ�ʼ״̬�������Ŀ��״̬����������£���������MOVE_NONE����ʶ
		//"�� ", "�� ", "�� ", "�� "
		
		execute1(&current, moves[i]);
		if (moves[i] == MOVE_NONE)

			break;

		switch (moves[i])

		{

		case MOVE_LEFT:

			cout << i << "��" << endl;

			break;

		case MOVE_RIGHT:

			cout << i << "��" << endl;

			break;

		case MOVE_UP:

			cout << i << "��" << endl;

			break;

		case MOVE_DOWN:

			cout << i << "��" << endl;

			break;

		}

		if (i>0&&i % 20 == 0) getchar();
	}

	cout << endl;
	return true;
}



//Ԥ�ȼ��������پ������(�����پ���Ϊ���ߵĺ�����֮��ľ���ֵ��������֮��ľ���ֵ֮��)

void cal_manhattan(void)

{

	for (int i = 0; i < SQUARES * SQUARES; i++)

	for (int j = 0; j < SQUARES * SQUARES; j++)

	{

		int tmp = 0;

		tmp += (absi(i / SQUARES - j / SQUARES) + absi(i % SQUARES - j % SQUARES));

		manhattan[i][j] = tmp;

	}

}

//�ָ��ı�
void split(string str1, string fg, vector<string> &array)
{
	int a = 0;
	int b = str1.find(fg, a);
	while (b >= 0)
	{
		array.push_back(str1.substr(a, b - a));
		a = b + fg.length();
		//Output("%d %d %s\n", b,i,list[i]);
		b = str1.find(fg, a);
	}
	array.push_back(str1.substr(a, str1.length() - a));
}


int main(int ac, char *av[])

{
	vector<string> array;

	split("1,20,3", ",", array);
	cout << array[0] << " " << array[1] << endl;

	//���������پ��룬���

	cal_manhattan();


	//����vector����װ��һ�������ľ���

	vector <int> tiles;

	//��������       

	//cin >> n;


	tiles.clear();

	//int num_init[] = { 21,18,6,24,2,19,9,7,22,1,20,17,13,5,10,4,11,23,0,12,15,3,16,14,8};

	//int num_init[] = { 1, 13, 2, 5, 15, 6, 3, 18, 4, 9, 8, 7, 17, 24, 10, 11, 12, 22, 23, 19, 16, 21, 14, 20, 0 };


	//int num_init[] = { 6, 1, 2, 4, 5, 10, 12, 3, 8, 15, 7, 17, 24, 9, 18, 22, 11, 13, 20, 14, 16, 21, 23, 19, 0 };

	//int num_init[] = { 7, 12, 14, 8, 5, 1, 2, 3, 15, 17, 9, 18, 10, 24, 4, 11, 16, 6, 20, 19, 21, 22, 13, 23, 0 };

	//int num_init[] = { 13, 11, 6, 3, 5, 2, 7, 12, 10, 4, 8, 22, 16, 15, 20, 19, 18, 9, 24, 1, 21, 23, 14, 17, 0 };


	int num_init[] = { 0, 16, 12, 15, 19, 13, 1, 4, 3, 18, 11, 24, 9, 7, 5, 22, 23, 17, 10, 14, 21, 6, 8, 2, 20 };

	//int num_init[] = { 2, 3, 7, 4, 6, 9, 5, 11, 13, 1, 10, 12, 14, 15, 8, 0 };

	//int num_init[] = { 7, 3, 5, 8, 6, 0, 4, 1, 2 };

	for (int i = 0; i < SQUARES * SQUARES; i++)

	{
		tiles.push_back(num_init[i]);

	}

	//�ж��Ƿ��н⣬�޽��������Ӧ��Ϣ���н���ʹ����Ӧ�㷨����
	int a[] = { 10, 8, 6,12,11,13,9};
	if (solvable(tiles)){
		for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++){
			START = clock();
			STEPSBOUND1 = a[i];
			if (solve_puzzle(*num_init,tiles)){
				cout << "�������кķѵĺ�������" << (clock() -START) << endl;
				break;
			}

		}
		
	}
	else{
		cout << "�޽�" << endl;
	}
	getchar();
	return 0;

}