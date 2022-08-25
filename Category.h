#pragma once
#include "Bin.h"
class Category
{
public:
	int id;
	int x1, x2;
	vector <Bin*> Bins;
	int catTime;
	int maxCap;
	int lastClosedBinId;
	void showCategory(string);
	void insertTtoB_FF(Task * t, char type);
	void insertTtoB_BF(Task * t, char type);
	void balanceTime(int);//Kanei update ta open-close kai epistrefei to 1o open
	Category();

	Category(int maxCap);
	~Category();
};