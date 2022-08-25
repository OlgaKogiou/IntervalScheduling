#pragma once
#include "KnapsackInterval.h"
#include "Task.h"
class KnapsackArray
{
public:
	float B;
	vector <Task *> S;
	KnapsackInterval * I;
	KnapsackArray();
	~KnapsackArray();
};