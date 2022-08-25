#pragma once
#include "Task.h"
#include "stdafx.h"
class Rectangle
{
public:
    bool deleted;
    //for global/local intersection between elements
    vector <Task *> tasksInSect;
    int x1, x2;
    float y1, y2;
    void showRectangle(string);
    bool recIntersectHM(Rectangle *);
    bool recIntersect(Rectangle *);
    bool floatEquals(float a, float b);
    Rectangle();
    Rectangle(int, int, float, float);
    ~Rectangle();
};