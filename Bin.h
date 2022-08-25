#pragma once
#include "Task.h"
#include "stdafx.h"
class Bin
{
public:
    int start, end, freeCap;
    bool open, freeCap_=1;
    //only online
    vector<Task*> TasksInside;
    Bin();
    ~Bin();
};