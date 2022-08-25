#include "stdafx.h"
#include "Point.h"


void Point::setEq(Point * p)
{
	this->s=p->s;
	this->c=p->c;
	this->d=p->d;
}

Point::Point()
{
}


Point::~Point()
{
	//cout << "~Point()\n";
}
