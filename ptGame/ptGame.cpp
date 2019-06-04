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



//开辟一个4*4的布局        

#define SQUARES 5

//将局面转换成整数的基数，类似于康托扩展

#define BASE 25                  

//DFS的最大搜索深度

#define DEPTHBOUND 50              

//路径的最大长度（假设不会超过50，为了让数组可以装填下）

#define STEPSBOUND 300                

int STEPSBOUND1 = 12;

#define STEPSBOUND2 3

#define MOVE_LEFT (-1)

#define MOVE_RIGHT 1

#define MOVE_UP (-SQUARES)

#define MOVE_DOWN SQUARES

#define MOVE_NONE 0



//开启一个曼哈顿距离的预算表       

int manhattan[SQUARES * SQUARES][SQUARES * SQUARES];

//后续的移动

int move1[SQUARES];



//一个描述局面信息的数据结构

struct node

{

	//利用vector容器装填当前的状态

	vector <int> state;

	//装填一个最优解，就是行走的序列

	int moves[STEPSBOUND];

	//当前的深度            

	int depth;

	//当前的节点的估计值（用于启发式搜索）

	int score;

	//空格的位置

	int blank;

};



//优先队列的比较函数,分值较小的在优先队列的前端(运算符重载)

bool operator<(node x, node y)

{

	return x.score > y.score;

}



//求绝对值

int absi(int x)

{

	return x >= 0 ? x : (-x);

}



//判断给定局面是否可解,利用上述的策略，就是逆序对的奇偶性原则

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

得到当前局面的后继走法。MOVE_LEFT = 向左移动空滑块，MOVE_RIGHT = 向右移动空滑块，

MOVE_UP = 向上移动空滑块，MOVE_DOWN = 向下移动空滑块

*/



void valid_moves(node &current)

{

	//获取后继走法，但除去退回到该状态的上一步的走法

	int last_move = MOVE_NONE;

	//搜索深度确定之后，就可以确定最后一步在哪里了

	if (current.depth)

		last_move = current.moves[current.depth - 1];

	memset(move1, MOVE_NONE, sizeof(move1));

	//尝试四个方向，这四个方向的移动都有范围的合法性制约

	if (current.blank % SQUARES > 0 && last_move != MOVE_RIGHT)

		move1[0] = MOVE_LEFT;

	if (current.blank % SQUARES < (SQUARES - 1) && last_move != MOVE_LEFT)

		move1[1] = MOVE_RIGHT;

	if (current.blank / SQUARES > 0 && last_move != MOVE_DOWN)

		move1[2] = MOVE_UP;

	if (current.blank / SQUARES < (SQUARES - 1) && last_move != MOVE_UP)

		move1[3] = MOVE_DOWN;

}



//将棋面状态转换为一个整数(保证每一个状态都有一个唯一的最优值)

unsigned long long key(vector <int> &tiles)

{

	unsigned long long key = 0;

	for (int i = 0; i < tiles.size(); i++)

		key = key * BASE + tiles[i];

	return key;

}



//从局面 current 执行 move 所指定的走法

node execute(node &current, int move)

{

	node successor;

	//走法步骤设定

	memcpy(successor.moves, current.moves, sizeof(current.moves));

	successor.depth = current.depth + 1;

	successor.moves[current.depth] = move;

	//局面状态设定,按 move 指定方向移动，交换空滑块位置(这里等于是交换两个方块的位置)

	successor.state = current.state;

	successor.blank = current.blank + move;

	successor.state[current.blank] = successor.state[successor.blank];

	successor.state[successor.blank] = 0;

	return successor;

}
int blanked = 24;

void execute1(int current[], int move)

{

	//局面状态设定,按 move 指定方向移动，交换空滑块位置(这里等于是交换两个方块的位置)

	

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

由于 h*（n） 在算法中非常关键，而且它是高度特化的，根据问题的不同而不同，所以需要找到一个合适

的 h*（n） 函数是比较困难的，在这里使用的是每个方块到其目标位置的曼哈顿距离之和，曼哈顿距离是

该状态要达到目标状态至少需要移动的步骤数。g*（n） 为到达此状态的深度，在这里采用了如下评估函

数： f*（n） = g*（n） + 4 / 3 * h*（n），h*（n） 为当前状态与目标状态的曼哈顿距离，亦可

以考虑计算曼哈顿配对距离。本题中实验了一下，效率比单纯曼哈顿距离要高，但比曼哈顿距离乘以适当系

数的方法低。可参考：

[Bernard Bauer，The Manhattan Pair Distance Heuristic for the 15-Puzzle，1994]

*/



//这里的评价函数综合地考虑到了曼哈顿距离和搜索的深度，所以比较完美

int score(vector <int> &state, int depth)

{

	int hn = 0;

	for (int i = 0; i < state.size(); i++)

	if (state[i] > 0)

		hn += manhattan[state[i] - 1][i];

	return (depth + STEPSBOUND1* hn / STEPSBOUND2);

}



//判断是否已达到目标状态。
int cc = 0;
bool solved(vector < int > &state)

{
	string t;
	//考虑最后一个元素是否是0,首先考虑最后一个元素，其实起到了一种剪枝的作用

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



//找到局面状态的空滑块位置

int find_blank(vector < int > &state)

{

	for (int i = 0; i < SQUARES * SQUARES; i++)

	if (state[i] == 0)

		return i;

}

/*

[IDA* 搜索]

深度优先在内存占用方面占优势，但是由于没有剪枝，导致搜索空间巨大，A* 搜索虽然剪枝，但是由于存

储了产生的每个节点，内存消耗较大。IDA* 搜索结合了两者的优势。IDA* 实质上就是在深度优先搜索的

算法上使用启发式函数对搜索深度进行限制

*/


clock_t START;

bool solve_puzzle_by_iterative_deepening_a_star(vector < int > tiles, int directions[])

{

	node copy;

	copy.state = tiles;

	copy.depth = 0;

	copy.blank = find_blank(tiles);

	memset(copy.moves, MOVE_NONE, sizeof(copy.moves));



	//检测当前局面是否为已解决状态

	if (solved(copy.state))

	{

		memcpy(directions, copy.moves, sizeof(copy.moves));

		return true;

	}



	//设定初始搜索深度为初始状态的评分

	int depth_limit = 0, min_depth = score(copy.state, 0);

	while (true)

	{

		//获取迭代后的评分

		if (depth_limit < min_depth)

			depth_limit = min_depth;

		else

			//开始搜索第一层

			depth_limit++;



		//开始新的深度优先搜素，深度为 depth_limit

		stack < node > open;

		open.push(copy);

		while (!open.empty())

		{

			node current = open.top();

			open.pop();

			//获取该局面的后继走法，后继走法都会加入开放集中

			valid_moves(current);

			for (int i = 0; i < SQUARES; i++)

			{

				if (move1[i] == MOVE_NONE)

					continue;

				//在当前局面上执行走法

				node successor = execute(current, move1[i]);

				//记录求解中前一步走法，如果找到解，那么立即退出。否则的话在限制的深度内将其加入开放集   

				if (solved(successor.state))

				{

					memcpy(directions, successor.moves, sizeof(successor.moves));

					return true;

				}

				//计算当前节点的评分，若小于限制，加入栈中，否则找超过的最小值

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

	//这里给出了BFS,DFS,A*和IDA*一共四种方法

	int moves[STEPSBOUND];

	// 深度优先搜索。

	// solve_puzzle_by_depth_first_search(tiles, moves);

	// 宽度优先搜索。

	// solve_puzzle_by_breadth_first_search(tiles, moves);

	// A* 搜索。解长度在 30 - 50 步之间的局面平均需要 7 s。UVa RT 1.004 s。

	// solve_puzzle_by_a_star(tiles, moves);

	// IDA* 搜索。解长度在 30 - 50 步之间的局面平均需要 1.5 s。UVa RT 0.320 s。

	if (!solve_puzzle_by_iterative_deepening_a_star(tiles, moves)) return false;

	// 输出走法步骤。

	for (int i = 0; i < STEPSBOUND; i++)

	{
		
		//有一种情况是初始状态本身就是目标状态，这种情况下，我们利用MOVE_NONE来标识
		//"↓ ", "↑ ", "→ ", "← "
		
		execute1(&current, moves[i]);
		if (moves[i] == MOVE_NONE)

			break;

		switch (moves[i])

		{

		case MOVE_LEFT:

			cout << i << "→" << endl;

			break;

		case MOVE_RIGHT:

			cout << i << "←" << endl;

			break;

		case MOVE_UP:

			cout << i << "↓" << endl;

			break;

		case MOVE_DOWN:

			cout << i << "↑" << endl;

			break;

		}

		if (i>0&&i % 20 == 0) getchar();
	}

	cout << endl;
	return true;
}



//预先计算曼哈顿距离填表(曼哈顿距离为两者的横坐标之差的绝对值与纵坐标之差的绝对值之和)

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

//分割文本
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

	//计算曼哈顿距离，填表。

	cal_manhattan();


	//利用vector容器装填一个完整的局面

	vector <int> tiles;

	//读入样例       

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

	//判断是否有解，无解则输出相应信息，有解则使用相应算法解题
	int a[] = { 10, 8, 6,12,11,13,9};
	if (solvable(tiles)){
		for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++){
			START = clock();
			STEPSBOUND1 = a[i];
			if (solve_puzzle(*num_init,tiles)){
				cout << "程序运行耗费的毫秒数：" << (clock() -START) << endl;
				break;
			}

		}
		
	}
	else{
		cout << "无解" << endl;
	}
	getchar();
	return 0;

}