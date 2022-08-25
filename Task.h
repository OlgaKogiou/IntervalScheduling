#pragma once
#include "stdafx.h"

class Task{
public:
    int id, start, end, length, machineId, demand, wait, startNoWait, initId;
    bool placed, checkOnlineDPB;
    float DBP_demand, placedAt;
    string smallLarge;
    double waitForExecution;

    int fakeD; //for Knapsack

    void showTask();
    Task(int, int);
    Task();
    ~Task();
};