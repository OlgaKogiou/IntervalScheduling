#pragma once
#include "Task.h"
#include "Edge.h"
#include "Point.h"
#include "KnapsackArray.h"
#include "Parameters.h"
class Knapsack
{
public:
	Parameters *prm;
	//string aglType;
	void adjustEdges();
	int id;
	vector<Edge*>* objList;
	float **matrix;
	int G;// to megethos tou sakou
	int K;//to megthos twn pros exetashs edges

	KnapsackArray *runWithRemove();
	KnapsackArray * runKnapsackEmpty();
	KnapsackArray * runKnapsackEmptyNoZ();
	KnapsackArray *runWithRemoveNoZ();

	KnapsackArray * runKnapsackEmptyInsideBen();

	KnapsackArray * runKnapsackFull(KnapsackArray *full);
	KnapsackArray * runKnapsackFullNoZ(KnapsackArray *full);

	Knapsack();
	Knapsack(Parameters *p);

	~Knapsack();
	
	KnapsackArray *sol;
	KnapsackArray *** arr;
	bool floatEquals(float a, float b);
	float objectBenefit(Task *n1, Task *n2, vector <Task *> *S);

	float objectBenefit_2(Task *n1, Task *n2, vector <Task *> *S,KnapsackInterval *Is);
	float objectBenefit_BIG(Task *n1, Task *n2, vector <Task *> *S);
	float objectBenefit_BIG_2(Task *n1, Task *n2, vector <Task *> *S);

	void printSolution();
	void delete2RowsBefore(int k);
};