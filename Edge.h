#pragma once
#include "Task.h"

class Edge
{
public:
	int catId;
	int placed;
	bool checked;
	Task *n1, *n2;
	int tasksOverlap, tasksUnion;
	float PEO;//percentage execution overlap
	float PEO_2;
	bool orphan;
	Edge();
	~Edge();
	void showEdge(string s);
};