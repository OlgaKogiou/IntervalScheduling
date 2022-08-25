#pragma once
#include "Task.h"
#include "stdafx.h"

class Parameters
{
public:
    
    long long int totalDuration;
    long long int span;
    ofstream openedMachines;
    int totalDemand, maxDuration, minDuration, maxCapacity, maxDemand, maxTasksNum /*-1 everything */, fileId, hete, heteId, maxMachinesParallel, actualTasksNum;
    int binsOpened; /*for launching cost to see if we can avoid openeing new bin*/
    int noOverlap;
    int categoryNumSpaa;
    int largest_i_for3_204_3;
    int thersholdValue;
    // int actual=0;
    // int category=0;

    clock_t begin, end;
    string dataFileName, algName, algType;
    double elapsed_secs, launchCostInit /*the cost f launcing a new bin*/, launchCost;
    double globalMfor3_204_3;

    float bParforHFF;
    vector <int> heteCores, machinesPerTimeSlot;
    vector<Task *> TasksSmall;
    vector<Task *> TasksBig;
    vector<Task *> Tasks;

    void findSpan();
    void createNewFile();
    void parseBig();
    void findGlobalX();

    Parameters();
    ~Parameters();

};