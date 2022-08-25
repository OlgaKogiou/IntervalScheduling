#include "stdafx.h"
#include "TSUSC.h"

//small to large
bool sortTasksOnlineStartOnline(const Task *i, const Task *j) { 
    return i->start < j->start; 
}

TSUSC::TSUSC()
{
}

TSUSC::~TSUSC()
{
}

// this is never being called
TSUSC::TSUSC(vector<Task*>* vec, string t, int maxCap)
{
	this->maxCap = maxCap;
	StringType = t;
	if (!(t == "20" || t == "21")) {
		cout << "wrong Type111\n";
		exit(0);
	}
	clocktsusc = 0;
	this->Tasks = vec;
	if (t == "123") { // we choose original FF
		originalFirstFit();
	}
	else {
		
		this->createCats();
		this->placement();
	}
}

TSUSC::TSUSC(Parameters *p)
{
	this->prm = p;
	
	prm->begin = clock();
	this->maxCap = p->maxCapacity; // max capacity of the bin
	StringType = p->algType; //which alg is going to run
	// if (!(p->algType == "110" || p->algType == "111" || p->algType == "101" || p->algType == "100" || p->algType == "123")) {
	// 	cout << "wrong Type111\n";
	// 	exit(0);
	// }
	clocktsusc = 0;
	this->Tasks = &p->Tasks;

	sort(Tasks->begin(), Tasks->end(), sortTasksOnlineStartOnline); // sort from small to large
	
    this->createCats(); // we need categories for the "classify by.." algo
	this->placement();

	prm->end = clock();
	prm->elapsed_secs = double(prm->end - prm->begin) / CLOCKS_PER_SEC;
}

void TSUSC::createCats(){
	Task *t;
    Category *cat;
	int minM, maxM;
    int catNum;

    maxM = std::numeric_limits<int>::min();
    minM = std::numeric_limits<int>::max();
    for (size_t i = 0;i <Tasks->size();i++) {
        t = Tasks->at(i);
        if (t->start < minM)
            minM = t->start;
        if (t->end > maxM)
            maxM = t->end;
    }
    a = prm->thersholdValue;
    int m = maxM / minM;
    float xx = (float)log2(m) / (float)log2(a);
    if(StringType[1]=='0')
        catNum =(int) ceil(xx) + 1;
    else if(StringType[1]=='1')
        catNum =(int) (ceil(xx) + 1)/a;
    for (int i = 0;i < catNum;i++) {
        cat = new Category(this->maxCap);
        cat->id = i;
        cat->x1 = ceil( pow(a,i)); // we start from i and not i-1 bc the loop starts from 0
        cat->x2 = ceil( pow(a,i+1));
        Cats.push_back(cat);
    }

}

void TSUSC::placement()
{
	Task *tt;
	cout << "ALL Categories: "<< Cats.size()<<"\n";

	int catId;
	for (size_t i = 0;i < Tasks->size();i++) { // we print the Tasks and show them their size
		tt = Tasks->at(i);
		if (i % 10000 == 0)
			cout << prm->fileId << " - " << i << " / " << Tasks->size() << "\n";		

		catId = -1;
		float mm;
		for (size_t j = 0;j < Cats.size();j++) { // we find the category that this Task belongs to 
			Category *cc = Cats[j];
			mm = (float)prm->maxDuration / (float)tt->length;
			if (mm >= cc->x1 && mm <= cc->x2) {
				catId = j;
				break;

			}

		}
		if (catId == -1) {
			catId = Cats.size() - 1;
		}
		
		Cats[catId]->insertTtoB_FF(tt, StringType[1]);

	}
}

void TSUSC::originalFirstFit()
{
	Task *tt;
	Category *cat = new Category(this->maxCap);
	cat->id = 0;
	Cats.push_back(cat);
	for (size_t i = 0;i < Tasks->size();i++) {
		
		tt = Tasks->at(i);
		Cats[0]->insertTtoB_FF(tt, '1');//'1' so as not to sort 
	}


}

void TSUSC::findResults()
{
	Task *tt;
	Category *cc;
	Bin *bb;
	//BUSY TIME
	totalBusyTime = 0;	
	long long int p1, p2;
	for (size_t i = 0;i < this->Tasks->size();i++) {
		if (!Tasks->at(i)->placed) {
			cout << "Not placed TSUSC\n";
			exit(0);
		}
	}
	
	for (size_t i = 0;i < this->Cats.size();i++) {
		cc = Cats[i];
		for (size_t j = 0;j < cc->Bins.size();j++) {
			bb = cc->Bins[j];
			for (size_t z = 0;z < bb->TasksInside.size();z++) {
				tt = bb->TasksInside[z];
				allPoints.push_back(tt->start);
				allPoints.push_back(tt->end);
			}
			sort(allPoints.begin(), allPoints.end());
			allPoints.erase(unique(allPoints.begin(), allPoints.end()), allPoints.end());
			int busy = 0;
			
			for (size_t z = 0;z < allPoints.size()-1;z++) {
				p1 = allPoints[z];
				p2 = allPoints[z + 1];
				for (size_t x = 0;x <  bb->TasksInside.size();x++) {
					tt = bb->TasksInside[x];
					if (tt->start<=p1&&tt->end>=p2) {
						busy += p2 - p1;
						totalBusyTime += (p2 - p1);
						if (p2 - p1 < 0) {
							cout << totalBusyTime << "\n";
							
							exit(0);
						}
						break;
					}
				}
			}		
			allPoints.clear();
			//cout << "Cat "<<i<< " Bin " << j  <<" Tasks: "<<bb->TasksInside.size()<< " busyTime " << busy << "\n";
		}		
	}

	cout<<"totalBusyTime " << totalBusyTime << "\n";
	numMachines = 0;
	int tasksNUM = 0;
	for (size_t i = 0;i < this->Cats.size();i++) {
		cc = Cats[i];
		numMachines += cc->Bins.size();
		for (size_t j = 0;j < cc->Bins.size();j++) {
			bb = cc->Bins[j];
			tasksNUM += bb->TasksInside.size();
		}
	}

	if (tasksNUM != Tasks->size()) {
		cout << "ERROR Tasks->size() + TasksB->size()\n";
		exit(0);
	}
	tasksPerMachines = (float)tasksNUM / (float)numMachines;
}
