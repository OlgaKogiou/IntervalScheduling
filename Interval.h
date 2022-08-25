#pragma once
#include "Task.h"
#include "Rectangle.h"
#include "stdafx.h"

class Interval{
public:
    Interval();
    Interval(int sx, int ex, float dy1, float dy2);
    ~Interval();
    int sx, ex;
    float dy1, dy2, m; //m:the demand of the interval
    vector <Task *> insctTasks;
    vector <Rectangle *>RLocal; // they contain all the rectangles that are R,B,U in h
    vector <Rectangle *>BLocal;
    vector <Rectangle *>ULocal;
    void showInterval();
};