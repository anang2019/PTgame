#include "stdafx.h"
#pragma warning( disable : 4996)


#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;

const int vol = 4;
const int counti = vol*vol;


class EightPuzzle
{
private:
	int num[counti];
	int malposition;
	int depth;
	int evaluation;
public:
	EightPuzzle *parent;
	EightPuzzle *leaf_last;
	EightPuzzle *leaf_next;
public:
	EightPuzzle(int *num_input);
	void init(int *target);
	void setNum(int num[]);
	int *getNum();
	void getNum(int *num);
	int getMalposition()
	{
		return this->malposition;
	}
	int getDepth()
	{
		return this->depth;
	}
	int getEvaluation()
	{
		return this->evaluation;
	}
	void print();
	bool solvable(int *target);
	bool find_target(int *target);
	EightPuzzle& operator=(EightPuzzle& eightPuzzle);
	EightPuzzle& operator=(int other_num[counti]);
	bool operator==(EightPuzzle& eigthPuzzle);
	bool operator==(int other_num[counti]);
};

EightPuzzle::EightPuzzle(int *num_input)
{
	int ii;
	for (ii = 0; ii<counti; ii++)
	{
		num[ii] = num_input[ii];
	}
	this->leaf_last = NULL;
	this->leaf_next = NULL;
	this->parent = NULL;
}

EightPuzzle& EightPuzzle::operator=(EightPuzzle& eightPuzzle)
{
	int ii;
	for (ii = 0; ii < counti; ii++)
	{
		this->num[ii] = eightPuzzle.getNum()[ii];
	}
	this->malposition = eightPuzzle.getMalposition();
	this->depth = eightPuzzle.getDepth() + 1;
	this->evaluation = this->malposition + this->depth;
	return *this;
}
EightPuzzle& EightPuzzle::operator=(int other_num[counti])
{
	int ii;
	for (ii = 0; ii < counti; ii++)
	{
		num[ii] = other_num[ii];
	}
	return *this;
}
bool EightPuzzle::operator==(EightPuzzle& eightPuzzle)
{
	int match = 1;
	int ii;
	for (ii = 0; ii < counti; ii++)
	{
		if (this->num[ii] != eightPuzzle.getNum()[ii])
		{
			match = 0;
			break;
		}
	}
	if (match == 0)
		return false;
	else
		return true;
}
bool EightPuzzle::operator==(int other_num[counti])
{
	int match = 1;
	int ii;
	for (ii = 0; ii < counti; ii++)
	{
		if (this->num[ii] != other_num[ii])
		{
			match = 0;
			break;
		}
	}
	if (match == 0)
		return false;
	else
		return true;
}

void EightPuzzle::init(int *target)
{
	int ii;
	int temp = 0;
	for (ii = 0; ii < counti; ii++)
	{
		if (num[ii] != target[ii])
		{
			temp++;
		}
	}
	this->malposition = temp;
	if (this->parent == NULL)
	{
		this->depth = 0;
	}
	else
	{
		this->depth = this->parent->depth + 1;
	}
	this->evaluation = this->malposition + this->depth;
}

void EightPuzzle::setNum(int num[])
{
	int ii;
	for (ii = 0; ii < counti; ii++)
	{
		this->num[ii] = num[ii];
	}
}

int *EightPuzzle::getNum()
{
	return this->num;
}

void EightPuzzle::getNum(int *num)
{
	int ii;
	for (ii = 0; ii < counti; ii++)
	{
		num[ii] = this->num[ii];
	}
}

bool EightPuzzle::solvable(int *target)
{
	int ii, ij;
	int count_num = 0, count_target = 0;
	for (ii = 0; ii < counti; ii++)
	{
		for (ij = 0; ij < ii; ij++)
		{
			if ((this->num[ij] < this->num[ii]) && (this->num[ij] != 0))
			{
				count_num++;
			}
			if (target[ij] < target[ii] && target[ij] != 0)
			{
				count_target++;
			}
		}
	}
	

	if ((count_num + count_target) % (2) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool EightPuzzle::find_target(int *target)
{
	int ii;
	for (ii = 0; ii < counti; ii++)
	{
		if (this->num[ii] != target[ii])
		{
			break;
		}
	}
	if (ii == counti)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool move_up(int *num)
{
	int ii;
	for (ii = 0; ii < counti; ii++)
	{
		if (num[ii] == 0)
		{
			break;
		}
	}
	if (ii < vol)
	{
		return false;
	}
	else
	{
		num[ii] = num[ii - vol];
		num[ii - vol] = 0;
	}
	return true;
}

bool move_down(int *num)
{
	int ii;
	for (ii = 0; ii < counti; ii++)
	{
		if (num[ii] == 0)
		{
			break;
		}
	}
	if (ii > vol*(vol-1)-1)
	{
		return false;
	}
	else
	{
		num[ii] = num[ii + vol];
		num[ii + vol] = 0;
	}
	return true;
}

bool move_left(int *num)
{
	int ii;
	for (ii = 0; ii < counti; ii++)
	{
		if (num[ii] == 0)
		{
			break;
		}
	}
	if (ii%vol == 0 )
	{
		return false;
	}
	else
	{
		num[ii] = num[ii - 1];
		num[ii - 1] = 0;
	}
	return true;
}

bool move_right(int *num)
{
	int ii;
	for (ii = 0; ii < counti; ii++)
	{
		if (num[ii] == 0)
		{
			break;
		}
	}
	if (ii%vol == vol-1)
	{
		return false;
	}
	else
	{
		num[ii] = num[ii + 1];
		num[ii + 1] = 0;
	}
	return true;
}

void EightPuzzle::print()
{
	int ii;
	for (ii = 0; ii<counti; ii++)
	{
		if ((ii + 1) % vol != 0)
		{
			cout << num[ii] << ",";
		}
		else
		{
			cout << num[ii] << endl;
		}
	}
}

bool existed(int *num, EightPuzzle *start)
{
	EightPuzzle *temp;
	for (temp = start; temp != NULL; temp = temp->parent)
	{
		if (*temp == num)
		{
			return true;
		}
	}
	return false;
}

EightPuzzle *best_route(EightPuzzle *start, EightPuzzle *target)
{
	EightPuzzle *temp, *best;
	temp = best = start;
	start->init(target->getNum());
	int min = start->getEvaluation();
	for (temp = start; temp != NULL; temp = temp->leaf_next)
	{
		if (min > temp->getEvaluation())
		{
			best = temp;
			min = temp->getEvaluation();
		}
	}
	return best;
}

void print_route(EightPuzzle *best, int list_length)
{
	int step = 0;
	EightPuzzle *temp;
	for (temp = best->parent; temp != NULL; temp = temp->parent)
	{
		cout << endl;
		temp->print();
		step++;
	}
	cout << endl << "总步数 " << step << "." << endl;
	cout << endl << "搜索状态数 " << list_length << "." << endl;
	return;
}

void proceeding(EightPuzzle &start, EightPuzzle &target)
{
	if (!start.solvable(target.getNum()))
	{
		cout << endl << "无解!" << endl;
		return;
	}

	EightPuzzle *best = &start;
	EightPuzzle *list = &start;
	EightPuzzle *apply, *temp;
	int num[counti], list_length = 0;
	while (best != NULL)
	{
		best = best_route(list, &target);
		if (best->find_target(target.getNum()))
		{
			print_route(best, list_length);
			cout << 2222 << endl;
			return;
		}
		temp = best->leaf_last;
		best->getNum(num);
		//把num重置成本层原样,没有按上下左右
		if (move_up(num) && !existed(num, best))
		{
			apply = new EightPuzzle(num);
			apply->parent = best;
			apply->init(target.getNum());
			apply->leaf_last = temp;
			if (temp == NULL)
			{
				list = apply;
			}
			else
			{
				temp->leaf_next = apply;
			}
			temp = apply;
			list_length++;
		}
		best->getNum(num);
		//把num重置成本层原样,没有按上下左右
		if (move_down(num) && !existed(num, best))
		{
			apply = new EightPuzzle(num);
			apply->parent = best;
			apply->init(target.getNum());
			apply->leaf_last = temp;
			if (temp == NULL)
			{
				list = apply;
			}
			else
			{
				temp->leaf_next = apply;
			}
			temp = apply;
			list_length++;
		}
		best->getNum(num);
		//把num重置成本层原样,没有按上下左右
		if (move_left(num) && !existed(num, best))
		{
			apply = new EightPuzzle(num);
			apply->parent = best;
			apply->init(target.getNum());
			apply->leaf_last = temp;
			if (temp == NULL)
			{
				list = apply;
			}
			else
			{
				temp->leaf_next = apply;
			}
			temp = apply;
			list_length++;
		}
		best->getNum(num);
		//把num重置成本层原样,没有按上下左右
		if (move_right(num) && !existed(num, best))
		{
			apply = new EightPuzzle(num);
			apply->parent = best;
			apply->init(target.getNum());
			apply->leaf_last = temp;
			if (temp == NULL)
			{
				list = apply;
			}
			else
			{
				temp->leaf_next = apply;
			}
			temp = apply;
			list_length++;
		}

		temp->leaf_next = best->leaf_next;

		if (best->leaf_next != NULL)
		{
			best->leaf_next->leaf_last = temp;
		}
		best->leaf_next = best->leaf_last = NULL;
	}
}



int main(int argc, char **argv)
{
	double time;
	clock_t START, FINISH;
	//int num_init[counti] = { 7, 3, 5, 8, 6, 2, 4, 1, 0 };
	int num_init[counti] = { 1, 12, 9, 7, 10, 2, 8, 4, 3, 0, 15, 14, 11, 13, 5, 6 };
	//int num_init[counti] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0, 15 };
	//int num_init[counti] = { 1, 2, 3, 4, 5, 6, 7, 0, 8 };
	//int num_init[counti] = { 2, 3, 1, 0 };

	//input(num_init);
	EightPuzzle *start = new EightPuzzle(num_init);
	//int num_target[counti] = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };

	//int num_target[counti] = { 1, 2, 3, 0 };

	int num_target[counti] = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,0 };

	EightPuzzle *target = new EightPuzzle(num_target);
	cout << "初始:" << endl;
	start->print();
	cout << "目标:" << endl;
	target->print();
	START = clock();
	proceeding(*start, *target);
	FINISH = clock();
	time = (double)(FINISH - START) * 1000 / CLOCKS_PER_SEC;
	cout << endl << "时间: ";
	cout << time << " 毫秒." << endl << endl;
	system("pause");
	return 0;
}
