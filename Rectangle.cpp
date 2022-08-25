#include "stdafx.h"
#include "Rectangle.h"
void Rectangle::showRectangle(string s) {
	cout << "[ " << x1 << "," << x2 << " | " << y1 << "," << y2 << "]"<<s;
}
bool Rectangle::recIntersect(Rectangle *r2)
{
	Rectangle *r1;
	r1 = this;
	bool noOverlap;
		
	noOverlap = r1->x1 >= r2->x2 ||
		r2->x1 >= r1->x2||
		r1->y1 > r2->y2 || floatEquals(r1->y1 , r2->y2) ||
		r2->y1 > r1->y2 || floatEquals(r2->y1 , r1->y2) ;
	
	return !noOverlap;
}
bool Rectangle::recIntersectHM(Rectangle *r2)
{
	if (r2->y2 > this->y2)
		return true;
	if (floatEquals(r2->y2, this->y2))
		return true;

	return false;

}
Rectangle::Rectangle(int x1, int x2, float y1, float y2) {
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
	deleted = false;
}

Rectangle::Rectangle()
{
	deleted = false;
}


Rectangle::~Rectangle()
{
}

bool Rectangle::floatEquals(float a, float b)
{
	return fabs(a - b) < std::numeric_limits<float>::epsilon();
}