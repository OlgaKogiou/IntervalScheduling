#include "stdafx.h"
#include "KnapsackArray.h"


KnapsackArray::KnapsackArray()
{
}


KnapsackArray::~KnapsackArray()
{
	//cout << "Delete Array\n";
	this->S.clear();
	vector<Task *>().swap(this->S);
	delete this->I;
}
