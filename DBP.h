#pragma once
#include "Task.h"
#include "Interval.h"
#include "Parameters.h"
#include "Bin.h"
#include "Category.h"
#include "stdafx.h"

class DBP{
public:
    Parameters *prm;
    int numMachines, totalBusyTime, numTasks, maxCap;
    float tasksPerMachines, maxHeight;
    vector <Bin *> Bins;
    vector <int> allPoints;
    vector <Task *> *Tasks, *TasksB;
    vector <Interval *> Intervals;
    vector <Category *> Cats;
    // they contain all the rectangles that are RBU
    vector <Rectangle *> RGlobal; 
    vector <Rectangle *> UGlobal;
    vector <Rectangle *> BGlobal;
    vector <Rectangle *> HyGlobal; //contains all the items that have been placed in the demand chart in "0101" algorithm
    DBP(vector<Task *> *vec, vector<Task *> *vecB);
    DBP(Parameters *p);
    DBP();
    ~DBP();
    void DDO(Parameters *p);
    void originalFirstFit();
    void originalBestFit();
    void findIntervals();
    void findIntervalsHybrid();
    void itemPlacement();
    void itemPlacementHybrid();
    void createBinsHybrid();
    void createNewM(float h, Interval *currInt);
    void addTasksToRectangle(Rectangle *);
    void balanceRectangles();
    void showPlacement();
    bool floatEquals(float a, float b);
    void createBins();
    void findResults();
    bool needBalance();
    float round(float d);
};