#pragma once
#include "Point.h"
#include "Task.h"
class KnapsackInterval
{
public:
	vector<Point *> P;
	int maxD;
	void copyIntervals(KnapsackInterval *in);
	void addInterval(Task * n);
	void addInterval2(Task * n);

	void showIntervals(string s);
	KnapsackInterval();
	~KnapsackInterval();
	bool floatEquals(float a, float b);

};