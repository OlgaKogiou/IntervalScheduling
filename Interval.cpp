#include "Interval.h"
#include "Rectangle.h"
#include "stdafx.h"

Interval::Interval(){

}

Interval::Interval(int sx, int ex, float dy1, float dy2){
    this->sx=sx;
    this->ex=ex;
    this->dy1=dy1;
    this->dy2=dy2;
    this->m=0.0;
}

void Interval::showInterval(){
    cout << "[" << sx << "~" << ex << "]" << " [" << dy1 << "~" << dy2 << "] m=" << m << "\n";
}

Interval::~Interval(){
    
}