#pragma once
#include "Knapsack.h"
#include "Edge.h"
#include "Parameters.h"
class Machine
{
public:

	//TSUSC major
	int startTime;
	string smallLarge;
	//~TSUSC major
	float machUtil;
	//ration
	int minDur;
	int maxDur;
	//~ration
    //output
	int numTFromPrevZ;
	//~output
    //ONLY FOR Zones
	int currPoint;//monoGiaZones;
    //~ONLY FOR Zones
	// output online
	int tottalTasksInside;
	long long int totalBusyTimeOnline;
	int occSpace;
	vector <Task *> tempInCnt;
	// ~output online
    KnapsackArray *kSol;
	int id;
	int cap;//den to allazw
	Knapsack *sack;
	vector <Edge *> *Edges;//ola ta edges den meiwnonai pote
	float **matrix;
	int onlineLastCheck;
	bool closed;
    Parameters *prm;
    int totalBusyTime;
    int freeSpace;

    //METHODS//
    //EDGES
	int insertEdgeInMachine(Edge *e);
	int insertEdgeInMachineTemp(Edge *e);
	int numberofTasksInside(Task *t);
    //~EDGES

    //OTHER METHODS
    void createKnapsack();
	void placeTask(Task *);
	int busyTime();
	void finndFreeSpace(int zoneWidth);
	void finndOccSpace();
    //~OTHER METHODS

	int remainingDemand(Task *t);
	int computeIntersection(Task *t);
	int swapReal(Task *t1, Task *t2);
	int addNewTaskReturnBusyTemp(Task *t);
	int removeTaskReturnBusyTemp(Task *t);
	void removeTaskReturnBusyReal(Task *t);
	void removeTaskReturnBusyRealIterator(Task *t);
	void addNewTaskReturnBusyReal(Task *t);

	// Constructors
	Machine();
	Machine(int id,int cap, vector <Edge *> *Edges, float **matrix,Parameters *p);
	~Machine();
    // ~Constructors

};