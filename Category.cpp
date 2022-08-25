#include "stdafx.h"
#include "Category.h"
//small to large
bool sortOpenTime(const Bin *i, const Bin *j) { 
    return (i->start < j->start); 
}

void Category::showCategory(string s)
{
	cout << id << " [" << x1 << " , " << x2 << "]" << s;
}

void Category::insertTtoB_FF(Task * t,char type) {
	//t->showTask();
	Bin *b, *Binsert;
	if(type=='0')
		std::sort(this->Bins.begin(), this->Bins.end(), sortOpenTime);

	if (Bins.empty()) {
		b = new Bin();
		//b->id = 0;
		b->freeCap = this->maxCap- t->demand;
		b->start = t->start;//t->start tha htan to idio
		//b->end = t->end;
		//b->open = true;
		b->TasksInside.push_back(t);
		t->placed = true;
		this->Bins.push_back(b);
	}
	else {
		Binsert = NULL;
		for (size_t i = 0;i < this->Bins.size();i++) {
			b = this->Bins[i];
			if (b->freeCap >= t->demand ) {
				Binsert = b;
				break;
			}
		}
		if (Binsert == NULL) {//den vrhka kanena prepei na ftiaksw ena
			b = new Bin();
		//	b->id = 0;
			b->freeCap =this->maxCap - t->demand;
			b->start = t->start;//t->start tha htan to idio
			//b->end = t->end;
			//b->open = true;
			b->TasksInside.push_back(t);
			t->placed = true;
			this->Bins.push_back(b);
		}
		else {
			Binsert->TasksInside.push_back(t);
			t->placed = true;
			//if(t->end>Binsert->end)
				//Binsert->end = t->end;
			Binsert->freeCap =Binsert->freeCap- t->demand;
		}
	}
	
}

void Category::insertTtoB_BF(Task * t, char type) {
	Bin *b, *Binsert;
	//std::sort(this->Bins.begin(), this->Bins.end(), sortOpenTime);
	// balanceTime(t->start);
	if (Bins.empty()) {
		b = new Bin();
		//b->id = 0;
		b->freeCap = this->maxCap- t->demand;
		b->start = t->start;//t->start tha htan to idio
		//b->end = t->end;
		//b->open = true;
		b->TasksInside.push_back(t);
		t->placed = true;
		this->Bins.push_back(b);
	}
	else {
		int min;
		Binsert = NULL;
		min = std::numeric_limits<int>::max();
		for (size_t i = 0;i < this->Bins.size();i++) {
			b = this->Bins[i];
			if (b->freeCap >= t->demand ) {
				if (b->freeCap - t->demand < min) {
					min = b->freeCap - t->demand;
					Binsert = b;
				}
			}
		}
		if (Binsert == NULL) {//den vrhka kanena prepei na ftiaksw ena
			b = new Bin();
			//	b->id = 0;
			b->freeCap = this->maxCap - t->demand;
			b->start = t->start;//t->start tha htan to idio
			//b->end = t->end;
			//b->open = true;
			b->TasksInside.push_back(t);
			t->placed = true;
			this->Bins.push_back(b);
		}
		else {
			Binsert->TasksInside.push_back(t);
			t->placed = true;
			//if (t->end>Binsert->end)
				//Binsert->end = t->end;
			Binsert->freeCap -= t->demand;
		}
	}
}

Category::Category()
{
	exit(0);
}
Category::Category(int maxCap)
{
	this->maxCap = maxCap;
	this->catTime = -1;
	this->lastClosedBinId = -1;
}


Category::~Category()
{
}

void Category::balanceTime(int clock)
{
	Bin *b;
	Task *t;
	// some tasks may have ended. offload bin
	for (size_t i = 0;i < Bins.size();i++) {
		b = Bins[i];
		for (size_t j = 0;j < b->TasksInside.size();j++) {
			t = b->TasksInside[j];
			if (!t->checkOnlineDPB) {
			// if ((clock >= t->end) && (!t->checkOnlineDPB)) {
				b->freeCap = b->freeCap + t->demand;
				t->checkOnlineDPB = true;
				if (b->freeCap > maxCap) {
					cout << "b->freeCap > maxCap\n";
					exit(0);
				}
					
			}
		}
		
	}
}
