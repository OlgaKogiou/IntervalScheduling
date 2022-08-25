#include "stdafx.h"
#include "DBPOnline.h"


// small to large
bool sortTasks1(const Task *i, const Task *j) { 
    if (i->start != j->start)
        return (i->start < j->start); 
    else { 
        return i->end < j->end; 
    } 
}

// small to large
bool sortTasksOnlineStartDBPOnline(const Task *i, const Task *j) { 
    return i->start < j->start; 
}

DBPOnline::DBPOnline()
{
}

DBPOnline::~DBPOnline()
{
}

// this is never being called
DBPOnline::DBPOnline(vector<Task*>* vec, string t, int maxCap)
{
	this->maxCap = maxCap;
	StringType = t;
	if (!(t == "110" || t == "111" || t == "101" || t == "100" || t == "123")) {
		cout << "wrong Type111\n";
		exit(0);
	}
	clockDBP = 0;
	this->Tasks = vec;
	if (t == "123") { // we choose original FF
		originalFirstFit();
	}
	else {
		
		this->createCats();
		this->placement();
	}
}

DBPOnline::DBPOnline(Parameters *p)
{
	this->prm = p;
	
	prm->begin = clock();
	this->maxCap = p->maxCapacity; // max capacity of the bin
	StringType = p->algType; //which alg is going to run
	// if (!(p->algType == "110" || p->algType == "111" || p->algType == "101" || p->algType == "100" || p->algType == "123")) {
	// 	cout << "wrong Type111\n";
	// 	exit(0);
	// }
	clockDBP = 0;
	this->Tasks = &p->Tasks;

	sort(Tasks->begin(), Tasks->end(), sortTasksOnlineStartDBPOnline); // sort from small to large
	if (p->algType == "123") {
		originalFirstFit();
	}
	else if (p->algType == "145"){
		originalBestFit();
	}
	else {

		this->createCats(); // we need categories for the "classify by.." algo
		this->placement();
	}
	prm->end = clock();
	prm->elapsed_secs = double(prm->end - prm->begin) / CLOCKS_PER_SEC;
}

void DBPOnline::createCats(){
	Task *t;
    Category *cat;
	a = 2;
	int minM, maxM;
	float xx;
	int numCats;
    if(StringType[1]== '0'){ //by departure
        // cout << "Not ready departure\n";exit(0);
        r = 10;
        int maxDep = std::numeric_limits<int>::min();
		int maxDur = std::numeric_limits<int>::min();
		int minDur = std::numeric_limits<int>::max();

        for (size_t i = 0;i < Tasks->size();i++) {
			if (Tasks->at(i)->end > maxDep)
				maxDep = Tasks->at(i)->end; // the max dept of Tasks
			if (Tasks->at(i)->length > maxDur)
				maxDur = Tasks->at(i)->length; //the max dur of Tasks
			if (Tasks->at(i)->length < minDur)
				minDur = Tasks->at(i)->length; //the min dur of Tasks
		}
        float mLitle = (float)maxDur / (float)minDur;
		xx = sqrt(mLitle)*minDur;
		r = (int)ceil(xx);
		xx = (float)maxDep / (float)r;
		if(prm->algType == "1011"){
			numCats = (float)log2(xx) / (float)log2(a) + 1;
		}
		if(prm->algType == "1012"){
			numCats = (float)(log2(xx) / (float)log2(a) + 1)/a;
		}
		numCats = (int)ceil(xx); // number of categories
		int intvls = 0;

        for (int i = 0;i < numCats;i++) {
			cat = new Category(this->maxCap);
			cat->id = i;
			if(prm->algType == "1011"){
				cat->x1 = ceil( pow(a,i)); // we start from i and not i-1 bc the loop starts from 0
				cat->x2 = ceil( pow(a,i+1));
			}
			else{
				cat->x1 = intvls;
				intvls += r;
				cat->x2 = intvls;
			}
			Cats.push_back(cat);
		}
    }
    else if(StringType[1]=='1'){///by duration
		maxM = std::numeric_limits<int>::min();
		minM = std::numeric_limits<int>::max();
		for (size_t i = 0;i <Tasks->size();i++) {
			t = Tasks->at(i);
			if (t->start < minM)
				minM = t->start;
			if (t->end > maxM)
				maxM = t->end;
		}
		int m = maxM - minM;
		xx = (float)log2(m) / (float)log2(a);
		int catNum =(int) ceil(xx) + 1;
	
		// catNum = prm->categoryNumSpaa;
		// a = prm->thersholdValue;

		for (int i = 0;i < catNum;i++) {
			cat = new Category(this->maxCap);
			cat->id = i;

			cat->x1 = ceil( pow(a,i)); // we start from i and not i-1 bc the loop starts from 0
			cat->x2 = ceil( pow(a,i+1));
			Cats.push_back(cat);
		}
	} else if (StringType[1]=='7'){ //Bucket First Fit
		maxM = std::numeric_limits<int>::min();
		minM = std::numeric_limits<int>::max();
		for (size_t i = 0;i <Tasks->size();i++) {
			t = Tasks->at(i);
			if (t->start < minM)
				minM = t->start;
			if (t->end > maxM)
				maxM = t->end;
		}
		int m = maxM - minM;
		xx = (float)((log2(m) / (float)log2(a)) + a + 3);
		int catNum =(int) ceil(xx) + 1;
		for (int i = 0;i < catNum;i++) {
			cat = new Category(this->maxCap);
			cat->id = i;

			cat->x1 = ceil( pow(a,i)); // we start from i and not i-1 bc the loop starts from 0
			cat->x2 = ceil( pow(a,i+1));
			Cats.push_back(cat);
		}
			
	}else {
		cout << "ERROR stringTypeCreateCats\n";exit(0);
	}
}

void DBPOnline::placement()
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
		
		if (StringType[2] == '0') {
			Cats[catId]->insertTtoB_FF(tt, StringType[1]);
		}
		else if (StringType[2] == '1') {
			// cout << "Not ready \n";exit(0);
			Cats[catId]->insertTtoB_BF(tt, StringType[1]);
		}
		else {
			cout << "ERROR stringType[1]\n";exit(0);
		}
	}
}

void DBPOnline::originalFirstFit()
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


void DBPOnline::originalBestFit()
{
	Task *tt;
	Category *cat = new Category(this->maxCap);
	cat->id = 0;
	Cats.push_back(cat);
	for (size_t i = 0;i < Tasks->size();i++) {
		
		tt = Tasks->at(i);
		Cats[0]->insertTtoB_BF(tt, '1');//'1' so as not to sort 
	}


}


void DBPOnline::findResults()
{
	Task *tt;
	Category *cc;
	Bin *bb;
	//BUSY TIME
	totalBusyTime = 0;	
	long long int p1, p2;
	for (size_t i = 0;i < this->Tasks->size();i++) {
		if (!Tasks->at(i)->placed) {
			cout << "Not placed DBPONLINE\n";
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
