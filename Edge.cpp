#include "stdafx.h"
#include "Edge.h"




Edge::Edge()
{
	orphan = false;
	checked = false;
	placed = false;
	catId = -1;
}


Edge::~Edge()
{
}

void Edge::showEdge(string s)
{
	cout << this->n1->id << " " << this->n2->id << " | " << this->PEO << s;
}