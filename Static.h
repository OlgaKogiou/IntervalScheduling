#pragma once
#include "Task.h"
#include "Edge.h"
#include "Knapsack.h"
#include "Parameters.h"

class VKBP_Static
{
public:
    int numMachines;
	int totalBusyTime;
	int machinesCapacity;
    int numTasks;
	float tasksPerMachines;
	float **matrix;
	vector <Task *> *Tasks;
	vector <Edge *> Edges;
	// vector <Machine *> Machines;
    vector <Edge *> hybridEdgesGreedy;
	vector <Edge *> hybridEdgesKnap;
	vector <Task *> unplaced;
	Parameters *prm;


    // METHODS //
    // Static
    void createEdges();
    void runStaticKnapsack();
    void runStaticKnapsackWithRemoves();
    int findOverlapPer(Task *t, KnapsackArray *temp);
	int findOverlapPer2(Task *t);
    void greedyStatic();
    void greedyStatic_2();
	void greedyStatic_BF_2();
    void greedyStatic_BF();
    void runStaticKnapsackHybrid(int perc);
    void splitEdges(int perc);
    void firstFit();
	void bestFit();
    void bestFit_2();
    // ~Static

    // Other methods
	void computeTotalBusyTime();
    bool floatEquals(float a, float b);
	void checkLegalPlacement();
    void assignWeights();
    // ~ Other methods


    // Constructors
	VKBP_Static();
	VKBP_Static(Parameters *p);
    // ~Constructors
	

	// Results
	void findResults();
	void findResultsZones();
	// ~Results


    // Local Search
	// int totalUnplaced;
	// int swapTemp(Machine *m1,Machine *m2,Task *t1, Task *t2);
	// int swapReal(int *fb,Machine *m1, Machine *m2, Task *t1, Task *t2);

	// int removeAdd(int fb, Machine *m, Task *t1, Task *t2);
	// void readKnapsack();
	// int numZZonesTemp;
	// void balanceMachines();
	// void twoOPT();
	// void transferWithNewMachines();
	// int transferTemp(int fb, Task *t1, Machine *m1, Machine*m2);
	// int tranferReal(int *fb, Task *t1, Machine *m1, Machine*m2);
	// ~ Local Search	

};

