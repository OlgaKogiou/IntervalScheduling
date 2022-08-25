#include "stdafx.h"
#include "Scheduler.h"
#include "DBP.h"
#include <fstream>

//small to llarge
bool sortTasksFF_File(const Task *i, const Task *j) { 
	return i->start< j->start; 
}
bool sortTasksFF_length(const Task *i, const Task *j) { 
	return i->length > j->length; 
}

void Scheduler::parseTasks()
{
	fstream myfile("data/" + prm->dataFileName, std::ios_base::in);
	if (!myfile.is_open()) {
		cout << "Error opening file\n" << prm->dataFileName << "\n";exit(0);
	}
	int s, dur, d, w;
	Task *t=NULL;
	int id = 0;
	int temp;
	bool cond = true;
	bool allOne = false;
	if (prm->maxDemand == -1) {
		cout << "All tasks DEMAND 1\n";
		allOne = true;
		prm->algName += "Dem1_";
	}
	this->prm->maxDemand = prm->maxCapacity - 1;
	while (!myfile.eof()) {
		myfile >> s >> w >> dur >> d;
		if (d <= this->prm->maxDemand && dur >0 && d>0 && cond) {
			
			if (w < 0)
				w = 0;
			if (allOne) {
				for (int i = 0;i < d;i++) {
					t = new Task(s + w, s + w + dur);
					t->wait = w;
					t->startNoWait = s;
					t->fakeD = 1;
					t->demand = 1;
					t->id = id++;
					t->initId = t->id;
					prm->Tasks.push_back(t);
					cond = id <  prm->maxTasksNum;
					if (prm->maxTasksNum == 0)
						cond = true;
				}
			}
			else {
				t = new Task(s + w, s + w + dur);
				t->wait = w;
				t->startNoWait = s;
				t->fakeD = d;
				t->demand = d;
				t->id = id++;
				t->initId = t->id;
				prm->Tasks.push_back(t);
				cond = id <  prm->maxTasksNum;
				if (prm->maxTasksNum == 0)
					cond = true;
			}


			if (prm->fileId == 19) {
				if (t->id == 9999) {
					break;
				}
			}

		}
	}

	for (int i = 0;i < prm->Tasks.size();i++) {
		t = prm->Tasks[i];
		if (t->fakeD <= prm->maxCapacity / prm->bParforHFF) {
			t->smallLarge = "small";
		}
		else {
			t->smallLarge = "large";
		}
	}
	sort(prm->Tasks.begin(), prm->Tasks.end(), sortTasksFF_File);
	prm->actualTasksNum = prm->Tasks.size();
	cout << "Parsing file (" << prm->fileId << ") " << prm->dataFileName << " #" << prm->actualTasksNum << " Tasks\n";
	myfile.close();
	

	prm->maxDuration = -1;
	prm->minDuration = std::numeric_limits<int>::max();
	for (int i = 0;i < prm->Tasks.size();i++) {
		t = prm->Tasks[i];
		
		if (t->length > prm->maxDuration)
			prm->maxDuration = t->length;
		if (t->length < prm->minDuration)
			prm->minDuration = t->length;
	}
	prm->globalMfor3_204_3 = prm->maxDuration / prm->minDuration;

	int ii = 0;
	while (1) {
		if (pow(2, ii) >= prm->maxDuration) {
			prm->largest_i_for3_204_3 = ii;
			break;
		}
		else {
			ii++;
		}
	}


}

void Scheduler::initDBP(string type){
    
	if(type[1] == '0'){
		parseTasks();
		prm->algName += "1_DBP_DD";
	}
    if (type[1] =='1'){
		ParseTasksDBP();
        prm->algName += "2_DBP_DC";
	}
    dbp = new DBP(prm); //here we initiate the group of algorithms
}

void Scheduler::initDBPOnline(string type) {
	parseTasks();
	if (type == "100")
		prm->algName += "3_DBP_Dep.FF";
	else if (type == "101")
		prm->algName += "3_DBP_Dep.BF";
	else if (type == "110")
		prm->algName += "4_DBP_Dur.FF";
	else if (type == "111")
		prm->algName += "4_DBP_Dur.BF";
	else if (type == "123")
		prm->algName += "5_DBP_ORIG_FF";
	else if (type == "1011"){
		prm->algName += "3&4_DBP_Dep&Dur.BF";
	}
	else if (type == "1012"){
		prm->algName += "3_DBP_Dep&ATP+RD.FF";
	}

	dbpOnline = new DBPOnline(prm);
}

void Scheduler::initTsusc(string type) {
	parseTasks();
	prm->findGlobalX();
	if (type == "20")
		prm->algName += "APT+AD.FF";
	else if (type == "21")
		prm->algName += "APT+RD.FF";

	tsusc = new TSUSC(prm);
}

// function that pareses tasks to two categories Small and Big
void Scheduler::ParseTasksDBP(){
fstream myfile("data/" + prm->dataFileName, std::ios_base::in);
	int temp;
	int s, dur, d, w;
	Task *t;
	int id = 0;
	bool cond = true;
	bool allOne = false;
	this->prm->maxDemand = prm->maxCapacity - 1;
	
	while (!myfile.eof()) {
		myfile >> s >> w >> dur >> d;
		if (d >= prm->maxCapacity && dur > 0 && d > 0 && cond) {
			//cout << "IN 1: ---";
			if (w < 0)
				w = 0;
			t = new Task(s + w, s + w + dur);
			t->fakeD = d;
			t->demand = d;
			d = prm->maxCapacity / 2;
			t->DBP_demand = round((float)d / (float)this->prm->maxCapacity);
			t->id = id++;
			if (t->DBP_demand <= 0.5)
				prm->TasksSmall.push_back(t);
			else
				prm->TasksBig.push_back(t);
			cond = id <  prm->maxTasksNum;
			if (prm->maxTasksNum == 0)
				cond = true;
			//t->showTask();
		}
		else if (d <= this->prm->maxDemand && dur >0 && d>0 && cond) {
			//cout << "IN 2: ---";
			if (w < 0) {
				w = 0;
			}
			t = new Task(s + w, s + w + dur);
			t->fakeD = d;
			t->demand = d;
			t->DBP_demand = round((float)d / (float)this->prm->maxCapacity);
			t->id = id++;
			if (t->DBP_demand <= 0.5)
				prm->TasksSmall.push_back(t);
			else
				prm->TasksBig.push_back(t);
			cond = id <  prm->maxTasksNum;
			if (prm->maxTasksNum == 0)
				cond = true;
			//t->showTask();
			}
	}

	prm->actualTasksNum = prm->TasksSmall.size() + prm->TasksBig.size();
	cout << "SMALL " << prm->TasksSmall.size() << " BIG: " << prm->TasksBig.size() <<"\n";
	cout << "Parsing file (" << prm->fileId << ") " << prm->dataFileName << " #" << prm->actualTasksNum << " Tasks\n";
	myfile.close();
}

float Scheduler::round(float d){
    //3 digits
    float t = ceilf(d*1000)/ 1000;
    return t;
}

Scheduler::Scheduler(){

}

Scheduler::Scheduler(Parameters *p){
    this->prm = p;
    // if(!(prm->extra == "-" || prm->extra == "NO" || prm->extra == "R" || prm->extra == "SE" || prm->extra == "STL")){
    //     cout << "wrong extra type";
    //     exit(0);
    // }

    //for hete machines
   

    ifstream myfile;
    string fileName;
    if(prm->hete == 10)
        fileName = "data/hete_10.txt";
    else if (prm->hete == 20)
        fileName = "data/hete_20.txt";
    else if (prm->hete == 30)
        fileName = "data/hete_30.txt";
    else if (prm->hete == 40)
        fileName = "data/hete_40.txt";
    myfile.open(fileName);
    int r;
    for (int i=0; i<100000; i++){
        myfile >> r;
        prm->heteCores.push_back(r);
    }

    myfile.close();
}

Scheduler::~Scheduler(){

}