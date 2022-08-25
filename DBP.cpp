#include "stdafx.h"
#include "DBP.h"
#include "Category.h"
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
bool sortTasks(const Task *i, const Task *j) { if (i->start != j->start)return (i->start < j->start); else { return i->end < j->end; } }//small to large
bool sortIntervals(const Interval *i, const Interval *j) { return (i->m > j->m); }//large to small
bool sortRect(const Rectangle *i, const Rectangle *j) { if (i->x1 != j->x1)return (i->x1 < j->x1); else { return i->x2 < j->x2; } }//small to large
bool IsToBeDeleted(Rectangle * i) { return (i->deleted==true); }


DBP::DBP(vector<Task *> *vec, vector<Task *> *vecB) {
	Tasks = vec;
	TasksB = vecB;
	this->numTasks = vec->size();
	this->findIntervals();
	
	this->itemPlacement();
	
	this -> createBins();

}
DBP::DBP(Parameters *p) {
	this->prm = p;
    Task  *tj;
	prm->begin = clock();
    this->Tasks = &p->Tasks;
    this->numTasks = (&p->Tasks)->size();
    this->maxCap = p->maxCapacity; // max capacity of the bin
    // std::sort(this->Tasks->begin(), this->Tasks->end(), sortTasks);
	
    if(p->algType == "000"){
        this->DDO(p);
        originalFirstFit();
    }
	else if(p->algType == "001"){
        this->DDO(p);
        originalBestFit();
    }
    else if(p->algType == "010"){
        Tasks = &p->TasksSmall;
        TasksB = &p->TasksBig;
        this->numTasks = (&p->TasksSmall)->size();
        this->findIntervals();
        this->itemPlacement();
        this->createBins();
    }
	//hybrid algprithm
	else if(p->algType == "0100"){
        Tasks = &p->TasksSmall;
        TasksB = &p->TasksBig;
        this->numTasks = (&p->TasksSmall)->size();
        this->findIntervals();
        this->itemPlacement(); // we place items in the demand chart
        this->createBinsHybrid();
		
    }
	else if(p->algType == "0101"){
		std::sort(this->Tasks->begin(), this->Tasks->end(), sortTasks);
		// this->DDO(p);
		Tasks = &p->TasksSmall;
        TasksB = &p->TasksBig;
		
		this->numTasks = (&p->TasksSmall)->size();
		this->findIntervalsHybrid();
		
		this->itemPlacementHybrid();
		this->createBins();
	}
    prm->end = clock();
    prm->elapsed_secs = double(prm->end - prm->begin) / CLOCKS_PER_SEC;
}
DBP::DBP() {
}
DBP::~DBP() {
}

void DBP::originalFirstFit()
{   
    // vector <Category *> Cats;
	Task *tt;
	Category *cat = new Category(this->maxCap);
	cat->id = 0;
	Cats.push_back(cat);
	for (size_t i = 0;i < Tasks->size();i++) {
		tt = Tasks->at(i);
		Cats[0]->insertTtoB_FF(tt, '1');//'1' so as not to sort 
	}

}

void DBP::originalBestFit()
{   
    // vector <Category *> Cats;
	Task *tt;
	Category *cat = new Category(this->maxCap);
	cat->id = 0;
	Cats.push_back(cat);
	for (size_t i = 0;i < Tasks->size();i++) {
		tt = Tasks->at(i);
		Cats[0]->insertTtoB_BF(tt, '1');//'1' so as not to sort 
	}

}

void DBP::findIntervals() {

	std::sort(this->Tasks->begin(), this->Tasks->end(), sortTasks);
	// find time points
	
	for (int i = 0;i < numTasks;i++) {
		allPoints.push_back(this->Tasks->at(i)->start);
		allPoints.push_back(this->Tasks->at(i)->end);
	}
	sort(allPoints.begin(), allPoints.end());
	allPoints.erase(unique(allPoints.begin(), allPoints.end()), allPoints.end());

	Task  *tj;
	int p1, p2;
	Interval *intNew;
	for (size_t i = 0;i < allPoints.size() - 1;i++) {
		p1 = allPoints[i];
		p2 = allPoints[i + 1];
		//create interval p1-p2
		intNew = new Interval(p1, p2, 0.0, 0.0);
		for (int j = 0;j < numTasks;j++) {
			tj = this->Tasks->at(j);
			if (tj->start >= p2) {
				break;
			}
			else {
				if (p1 >= tj->end) {//they dont intersect. tj is sooner than p12

				}
				else {
					intNew->m += tj->DBP_demand;
					//intNew->insctTasks.push_back(tj);
				}
			}
		}
		intNew->dy2 = intNew->m;
		if (!floatEquals(intNew->m, 0.0)) {
			Intervals.push_back(intNew);
		}		
	}
}

void DBP::findIntervalsHybrid(){
	Task  *tj;
	int p1, p2;
	Interval *intNew;
	for (int i = 0;i < numTasks;i++) {
		intNew = new Interval(0.0, this->Tasks->at(i)->end - this->Tasks->at(i)->start, 0.0, this->Tasks->at(i)->DBP_demand);
		intNew->m = this->Tasks->at(i)->DBP_demand;
		if (!floatEquals(intNew->m, 0.0)) {
			Intervals.push_back(intNew);
		}
		
	}

}

void DBP::DDO(Parameters *p){
    Task* tj;
    // this->Tasks = &p->TasksSmall;
	this->Tasks = &p->Tasks;
	
    
    for (int i = 0;i < p->actualTasksNum;i++)   //  'for' loop is used for sorting the numbers in descending order
    {
        for (int j = 0;j < p->actualTasksNum;j++)
        {
            if ((this->Tasks->at(i)->end - this->Tasks->at(i)->start) < (this->Tasks->at(j)->end - this->Tasks->at(j)->start))
            {   
                tj = this->Tasks->at(i);
                this->Tasks->at(i) = this->Tasks->at(j);
                this->Tasks->at(j) = tj;
            }
        }
    }
}

void DBP::itemPlacement(){
	
	//udpate GLobal U R(empty), B(empty)
	Rectangle *rr;
	Interval *intT;
	for (size_t i = 0;i < Intervals.size();i++) {
		intT = Intervals[i];
		rr = new Rectangle(intT->sx,intT->ex,intT->dy1,intT->dy2);
		UGlobal.push_back(rr);
		addTasksToRectangle(rr);
	}
	sort(this->UGlobal.begin(), UGlobal.end(), sortRect);
	//-------------------------------
	//sort M
	std::sort(this->Intervals.begin(), this->Intervals.end(), sortIntervals);
	maxHeight = Intervals[0]->m;
	//cout << "M= " << Intervals[0]->m << "\n";
	Rectangle * h=new Rectangle();//tha kanw th grammh h rectangle gia na mporw na vrw eukola intersections
	Interval *currInt;// einai sthn ousia to interval pou exei mesa tou to current max M
	Rectangle *IU;
	Rectangle *taskToRect = new Rectangle();// otan exw to task pou isws na kanw place to kanw rectangle gia na vriskw intersections
	Rectangle *newBlue, *newRed;
	/*
	for (int i = 0;i < this->Intervals.size();i++) {
		currInt = Intervals[i];
		currInt->showInterval();
	}	
	for (int j = 0;j < UGlobal.size();j++) {
		cout << "U {" << UGlobal[j]->x1 << "," << UGlobal[j]->x2 << "} {"<<UGlobal[j]->y1<<","<<UGlobal[j]->y2<<" ";
		cout << "[";
		for (int z = 0;z < UGlobal[j]->tasksInSect.size();z++) {
			Task *t = UGlobal[j]->tasksInSect[z];
			cout << t->id << " ";
		}cout << "]\n";			
	}cout << " \n";
	getchar();
	*/
	int plotCnt = 0;
	do {
		currInt = Intervals[0];
		if (plotCnt % 100==0) {
			cout << "PHASE A Current Heigth: " << currInt->m << " ";
			int ttt = 0;
			for (size_t i = 0;i < Tasks->size();i++) {
				if (Tasks->at(i)->placed == true)
					ttt++;
			}
			cout << ttt << "/" << Tasks->size() << "\n";
		}
	
		plotCnt++;
		h->x1 = 0;h->x2 = allPoints[allPoints.size() - 1];
		h->y1 = currInt->m; h->y2 = h->y1;		
		
		for (size_t i = 0;i < RGlobal.size();i++) {
			rr = RGlobal[i];
			if (h->recIntersectHM(rr)) {//intervals Intersect with line h
				currInt->RLocal.push_back(rr);
				//rr->showRectangle(",");
			}
		}
		//cout << "\nBlue Local: \n";
		for (size_t i = 0;i < BGlobal.size();i++) {
			rr = BGlobal[i];
			if (h->recIntersectHM(rr)) {//intervals Intersect with line h
				currInt->BLocal.push_back(rr);
				//rr->showRectangle(",");
			}
		}
		//cout << "\nU Local: \n";
		for (size_t i = 0;i < UGlobal.size();i++) {
			rr = UGlobal[i];			
			//rr->showRectangle();
			if (h->recIntersectHM(rr)) {//intervals Intersect with line h
				currInt->ULocal.push_back(rr);
				//rr->showRectangle(",");
			}
		}
		//cout << "\n";
		
	//	cout << "CURR INTERVAL TASK: ";

		if (UGlobal.empty()) {
			goto emptyUGLOBAl;
		}
		while (!currInt->ULocal.empty()) {			
			IU= currInt->ULocal[0];
//cout << "PHACE B height " << currInt->m << " IU ";
//IU->showRectangle("\n");
			if (IU->deleted) {
				cout << "exw thema me to IU\n";exit(0);
			}
			Task *selected = NULL;
			/*cout << "ALL Us: ";
			or (int i = 0;i < currInt->ULocal.size();i++) {
				if(!currInt->ULocal[i]->deleted)
					currInt->ULocal[i]->showRectangle(",");
			}
			cout << "\nIU: ";
			IU->showRectangle("\n");		
			
			cout << "Intrsect with IU ";
			for (int i = 0;i < IU->tasksInSect.size();i++) {
				if (!IU->tasksInSect[i]->placed)
					cout << IU->tasksInSect[i]->id << " ";
			}
			getchar();			
			*/
			//cout << "\nIU: ";IU->showRectangle("\n");
			/*cout << "Red Local: ";
			for (int i = 0;i < RGlobal.size();i++) {
				rr = RGlobal[i];rr->showRectangle(",");
			}cout << "\n";
			getchar();
			*/
			bool intersectRED;
			bool intersectU;
			bool beRed = false;
			for (size_t i = 0;i < IU->tasksInSect.size();i++) {
				if (!IU->tasksInSect[i]->placed) {
					
					selected = IU->tasksInSect[i];
					
					intersectRED = false;
					intersectU = false;
					//cout << "Selected: " << selected->id <<" "<< IU->tasksInSect[i] << "\n";
					taskToRect->x1 = selected->start;taskToRect->x2 = selected->end;
					taskToRect->y1 = round(currInt->m-selected->DBP_demand);
					taskToRect->y2 = currInt->m;
					for (size_t j = 0;j < currInt->RLocal.size();j++) {//if it doesnt intersect with RED
						if (taskToRect->recIntersect(currInt->RLocal[j])) {
							intersectRED = true;break;
						}
					}
					if (currInt->ULocal.size() != 1) {//an exei ena stoixeio to kanei sigoura intersect
						for (size_t j = 1;j < currInt->ULocal.size();j++) {// ksekinaw apo 1 giati to 0 to exw sto IU
							if (taskToRect->recIntersect(currInt->ULocal[j])&& !currInt->ULocal[j]->deleted) {

								intersectU = true;break;
							}
						}
					}
					//cout << "Intersect RED=" << intersectRED << " U=" << intersectU << "\n";
					if ( !intersectRED && !intersectU) {
						beRed = true;
						break;
					}					
				}
			}			
			
			if (!beRed) {//prepei na ginei BLUE
				//cout << "EGINE BLUE\n";
			
				newBlue = new Rectangle();

				newBlue->x1 = IU->x1;newBlue->x2 = IU->x2;
				newBlue->y1 = 0.0; newBlue->y2 = IU->y2;
				BGlobal.push_back(newBlue);
				currInt->BLocal.push_back(newBlue);
						//cout << "currInt ";currInt->showInterval();
						//cout << "newBlue ";newBlue->showRectangle("\n");
				currInt->ULocal.erase(currInt->ULocal.begin());
				
				//remove that IU from UGlobal as it is Blue now
				int pos = -1;
				for (size_t i = 0;i < UGlobal.size();i++) {
					if (UGlobal[i] == IU) {//apo U egine Blue ara antio..
						pos = i; 
						break;
					}
				}
				if(pos!=-1)
					UGlobal.erase(UGlobal.begin()+pos);
				else {
					cout << "BLUE DELETE U ERROR\n";exit(0);
				}
			}else{//tha ginei RED
				//cout << " \n------- RED -----------\n";
				//prepei na ginei RED
				selected->placed = true;
				selected->placedAt = h->y1;
				//cout << "placed at " << selected->placedAt << "| ";
				//selected->showTask();
				
				//cout << "Placed: " << selected->id << "\n";
				newRed = new Rectangle();
				//calculate  the intersected intervals
				int xmin,xmax;
				if (selected->start > IU->x1)
					xmin = selected->start;
				else
					xmin = IU->x1;
				if (selected->end > IU->x2)
					xmax = IU->x2;
				else
					xmax = selected->end;

				newRed->x1 = xmin;newRed->x2 = xmax;
				newRed->y1 =round( h->y2 - selected->DBP_demand);
				
				newRed->y2 = h->y2;
				if (newRed->y1 < 0 &&newRed->y1>-0.01) 
				{
					cout << "ARNHTIKO\n";
					cout << newRed->y1 << "\n";
					//newRed->showRectangle("\n");
					//exit(0);
					//sgetchar();
					newRed->y1 = 0.0;
				}
				else if(newRed->y1 < 0){
					cout << "BIG ARNHTIKO \n";
					cout << newRed->y1 << "\n";
					//newRed->showRectangle("\n");
					newRed->y1 = 0.0;
					//getchar();
					//exit(0);
					
				}
				RGlobal.push_back(newRed);
				currInt->RLocal.push_back(newRed);// to vazw kai sto local wste giati mporei sth while na to tsekarw gia allo task
				float oldHeight = IU->y2;
				IU->y2 = newRed->y1;//meiwthke to U xwros (vevaia edw mporei an eprepe na allaksei to aristera deksia alla den exei diafora. 				
				//to IU tha fygei apo ta locals alla apo global yparxe giati mporei na mhn kalyfthhke olo
				currInt->ULocal.erase(currInt->ULocal.begin());
				//remove that IU apo ta GLOBALs sthn periptwsh pou to IU epese sto 0,0 sto ypsos
				if (floatEquals(IU->y1,0.0) && floatEquals(IU->y2,0.0)) {
					int pos = -1;
					for (size_t i = 0;i < UGlobal.size();i++) {
						if (UGlobal[i] == IU) {
							pos = i;
							break;
						}
					}
					if (pos != -1)
						UGlobal.erase(UGlobal.begin() + pos);
					else {
						cout << "IU PROS DELETE U ERROR\n";exit(0);
					}
				}
				
				Rectangle *newU;
				if (IU->x1 < selected->start) {
					//cout << "NEW U 1o\n";
					//cout << "currInt ";currInt->showInterval();
					//getchar();
					newU = new Rectangle();
					newU->x1 = IU->x1; newU->x2 = selected->start;
					newU->y1 = IU->y1; newU->y2 = oldHeight;
					UGlobal.push_back(newU);
					currInt->ULocal.push_back(newU);
					addTasksToRectangle(newU);
				}
				if (IU->x2 > selected->end) {
					//cout << "NEW U 2o\n";
					//cout << "currInt ";currInt->showInterval();
					//getchar();
					newU = new Rectangle();
					newU->x1 = selected->end; newU->x2 = IU->x2;
					newU->y1 = IU->y1; newU->y2 = oldHeight;
					UGlobal.push_back(newU);
					currInt->ULocal.push_back(newU);
					addTasksToRectangle(newU);
					//den kanw balance edw giati tha vriskomai sto idio ypsos ara an xwraei to task na mpei
					// to geitoniko diasthma exei mikroter ypsos pleon
					//an vrethoun sekoino ypsos argotera?? Oxi, afou h eytheia an pesei pros ta katw ta nea U den tha yparxoun
				}
				bool notBelongM = false;
				float newMh = round(h->y1 - selected->DBP_demand);
				for (size_t i = 0;i < Intervals.size();i++) {
					if (floatEquals(Intervals[i]->m , newMh )|| Intervals[i]->m == newMh) {
						notBelongM = true;
					}
				}					
				//cout << "\th->y1 - selected->DBP_demand " << newMh << "\n";
				if (h->y1 > selected->DBP_demand && !notBelongM) {//add h into M
					//cout << "\tCreate new M "<< newMh<<"\n";
					createNewM(newMh, currInt);
					// den vazw sthn createNewM tasks mesa giati ftiaxnw neo ypsos. apo auto to ypsos argotera tha dw pia GLOBAL einai mesa!
					std::sort(this->Intervals.begin(), this->Intervals.end(), sortIntervals);
				}
				//getchar();
			}
			//cout << "END B";//getchar();
		}
		balanceRectangles();
		{
			vector <Rectangle *>temp;

			for (size_t i = 0;i < UGlobal.size();i++) {
				if (!UGlobal[i]->deleted)
					temp.push_back(UGlobal[i]);
			}
			UGlobal.clear();
			for (size_t i = 0;i < temp.size();i++) {
				UGlobal.push_back(temp[i]);
			}
		}
		//delete h from M
	emptyUGLOBAl:
		currInt->RLocal.clear();currInt->BLocal.clear();currInt->ULocal.clear();
		currInt->insctTasks.clear();
		Intervals.erase(Intervals.begin());
		
		//cout << "END A";//getchar();
	} while (!Intervals.empty());
	/*
	cout << "\t\t---------------FINAL PLACEMENT----------------\n";
	
	cout << "Red GLOBAL: \n";
	for (int i = 0;i < RGlobal.size();i++) {
		rr = RGlobal[i];	rr->showRectangle(",");
	}cout << "\n";
	cout << "Blue GLOBAL: \n";
	for (int i = 0;i < BGlobal.size();i++) {
		rr = BGlobal[i];	rr->showRectangle(",");
	}cout << "\n";
	cout << "U GLOBAL: \n";
	for (int i = 0;i < UGlobal.size();i++) {
		rr = UGlobal[i];	rr->showRectangle(",");
	}cout << "\n";
	*/
}

void DBP::itemPlacementHybrid(){
	Rectangle *rr;
	Interval *intT;
	Task* t;
	int heuristicStop = 0, length=0, maxDur, minDur;
	float maxHeight = 0.0, h =0.0;

	maxDur = std::numeric_limits<int>::min();
	minDur = std::numeric_limits<int>::max();

	for (size_t i = 0;i < Intervals.size();i++) {
		intT = Intervals[i];
		if (intT->ex- intT->sx > maxDur)
			maxDur = intT->ex- intT->sx; //the max dur of Tasks
		if (intT->ex- intT->sx < minDur)
			minDur = intT->ex- intT->sx; //the min dur of Tasks
		heuristicStop += (intT->ex - intT->sx);
		rr = new Rectangle(intT->sx,intT->ex,intT->dy1,intT->dy2);
		addTasksToRectangle(rr);
	}
	cout << "maxDur" << maxDur << "\n";
	cout << "minDur" << minDur << "\n";
	cout << "maxDur/minDur" << maxDur/minDur << "\n";
	heuristicStop = maxDur; // /(minDur*(heuristicStop)/ (maxDur/minDur)); // m= maxDur/minDur
	std::cout << heuristicStop << " heuristic stop\n";
	// cout << prm->maxDuration / prm->minDuration << " m\n";
	//-----------------------------------------------------
	
	length=0;
	int i =0;
	while(i!=numTasks) {
		intT = Intervals[i];
		t = this->Tasks->at(i);
		i++;
		
		length = length + (intT->ex - intT->sx);
		if (length>heuristicStop){
			h = (float)(h + maxHeight);
			length = 0;
			maxHeight = 0;
		}
		t->placedAt = (float)(intT->dy2 + h);
		if (maxHeight < (intT->dy2 - intT->dy1)){
				maxHeight= (intT->dy2 - intT->dy1);
				
		}	

	}
	this->maxHeight = h + maxHeight;
}

void DBP::createNewM(float h, Interval * currInt)
{
	Interval *newM = new Interval();
	newM->m = h;
	newM->sx = currInt->sx;newM->ex = currInt->ex;
	newM->dy1 = currInt->dy1;
	newM->dy2 = h;
	// ma tou valw mesa kai olla ta tasks
	Task *tj;
	int p1, p2;
	p1 = newM->sx;
	p2 = newM->ex;
	for (int j = 0;j < numTasks;j++) {
		tj = this->Tasks->at(j);
		if (tj->start >= p2) {
			break;
		}
		else {
			if (p1 >= tj->end) {//they dont intersect. tj is sooner than p12
			}
			else {
				newM->insctTasks.push_back(tj);
			}
		}
	}
	Intervals.push_back(newM);
}

void DBP::addTasksToRectangle(Rectangle * r) {
	Task *tj;
	int p1, p2;
	p1 = r->x1;
	p2 = r->x2;
	
	for (int j = 0;j < numTasks;j++) {
		tj = this->Tasks->at(j);
		if (tj->start >= p2) {
			break;
		}
		else {
			if (p1 >= tj->end) {//they dont intersect. tj is sooner than p12
			}
			else {
				r->tasksInSect.push_back(tj);
			}
		}
	}
}

void DBP::balanceRectangles()
{
	
	while(needBalance()) {
		
	}
	
		
}

void DBP::showPlacement()
{
	Task *t;
	for (size_t i = 0;i < Tasks->size();i++) {
		t = Tasks->at(i);
		std::cout << t->id << " " << t->placedAt << "\n";
	}
}

bool DBP::floatEquals(float a, float b)
{
	//cout << "fabs(a - b) " << fabs(a - b) << "\n";
	//cout << "epsilon() " << std::numeric_limits<float>::epsilon() << "\n";
	return fabs(a - b) < std::numeric_limits<float>::epsilon();


	
}

void DBP::createBins()
{
	int m = (int) ceil(2 * maxHeight);
	Bin *b;
	for (int i = 0;i < 2 * m - 1;i++) {
		b = new Bin();
		Bins.push_back(b);
	}
	float h;
	Task *t;
	int binIndex;
	float hMsr;
	float kM1D2;
	//small Tasks
	bool findBin = false;
	for (size_t i = 0;i < Tasks->size();i++) {
		bool findBin = false;
		t = Tasks->at(i);
		h = t->placedAt;
		binIndex;
		hMsr = round(h - t->DBP_demand);
		for (int k = 1;k <= m;k++) {
			kM1D2 = round((float)(k - (float)1.0) /(float) 2.0);
			if ((kM1D2 <= hMsr || floatEquals(kM1D2, hMsr))
				&& hMsr < h
				&& (h <= k / (float)2.0 || floatEquals(h, k / (float) 2.0))) {
				binIndex = k - 1;
				Bins[binIndex]->TasksInside.push_back(t);findBin = true;
				goto endfor;
			}
		}

		for (int k = 1;k < m;k++) {
			if (hMsr < k / (float)2.0 &&k / (float)2.0 < h) {
				binIndex = m+k-1;
				Bins[binIndex]->TasksInside.push_back(t);findBin = true;
				goto endfor;
			}
		}
		endfor:
			cout << "";
	
			if (!findBin) {
				cout << "ERROR to task dn mphke se BIn\n";
				b = new Bin();
				b->TasksInside.push_back(t);
				t->placed = true;
				Bins.push_back(b);
				//t->showTask();
				//exit(0);
			}
	}
	//large Tasks
	for (size_t i = 0;i < TasksB->size();i++) {
		t = TasksB->at(i);
		b = new Bin();
		b->TasksInside.push_back(t);
		t->placed = true;
		t->placedAt = t->DBP_demand;
		Bins.push_back(b);
	}

}

void DBP::findResults()
{
	
	allPoints.clear();
	Task *tt;
	Bin *bb;
    Category *cc;
	//BUSY TIME
	totalBusyTime = 0;
	int p1, p2;
	int totalTasks = 0, binsSize;

    if(prm->algType[1] == '1'){

        for (int i = 0;i < Bins.size();i++) {
            bb = Bins[i];
            std::cout << "Bin " << i << " #Tasks " << bb->TasksInside.size()<<" ";
            std::cout << "FreeCap: " << bb->freeCap << "\n";
            totalTasks += bb->TasksInside.size();
            
        }
        std::cout << "totalTasks " << totalTasks << "\n";
        binsSize = Bins.size();
    }
    else if(prm->algType[1] == '0'){
        cc = Cats[0];
        binsSize = cc->Bins.size();
    }
    int busy = 0;
	vector <float> maxD;
	for (size_t i = 0;i < binsSize; i++) {
        if(prm->algType[1] == '1'){
            bb = Bins[i];
            if (bb->TasksInside.empty())
			    goto emptyBin;
        }
		    
        else if(prm->algType[1] == '0')
            bb = cc->Bins[i];
		
		for (size_t j = 0;j < bb->TasksInside.size();j++) {
			tt = bb->TasksInside[j];
			allPoints.push_back(tt->start);
			allPoints.push_back(tt->end);
		}
		sort(allPoints.begin(), allPoints.end());
		allPoints.erase(unique(allPoints.begin(), allPoints.end()), allPoints.end());
		
		for (size_t z = 0;z < allPoints.size() - 1;z++) {
			float md = 0;
			p1 = allPoints[z];
			p2 = allPoints[z + 1];

			for (size_t x = 0;x < bb->TasksInside.size();x++) {
				tt = bb->TasksInside[x];
				//cout << "Bin " << i << " "; tt->showTask();
				if (tt->start <= p1 && tt->end >= p2){
					busy += p2 - p1;
					md += tt->DBP_demand;
					totalBusyTime += p2 - p1;
                    if(prm->algType[1] == '0'){
                        if (p2 - p1 < 0) {
                                std::cout << totalBusyTime << "\n";
                                exit(0);
                        }
                        break;
                    }
				}
			}
			maxD.push_back(md);
		}
		allPoints.clear();
	emptyBin:
			std::cout << "";
            // if(prm->algType[1] == '1'){
            //     float maxBinDemand;
            //     if (maxD.empty()) {
            //         maxBinDemand = 0.0;
            //     }
            //     else {
            //         maxBinDemand = *std::max_element(maxD.begin(), maxD.end());
            //         maxD.clear();
            //     }
                
            //     if (maxBinDemand > 1.0) {
            //         cout << "ERROR maxBinDemand ";
            //         exit(0);
            //     }
            // }
	}

    // if(prm->algType[1] == '0')
    std::cout<<"totalBusyTime " << totalBusyTime << "\n";
	
    numMachines = 0;
	int tasksNUM = 0;
    if(prm->algType[1] == '0'){
        numMachines += cc->Bins.size();
    }
	for (size_t i = 0;i < binsSize;i++) {
        if(prm->algType[1] == '0')
            bb = cc->Bins[i];
        else if(prm->algType[1] == '1'){
            bb = Bins[i];
            if (!bb->TasksInside.empty()) {
			    numMachines++;
		    }
        }
		tasksNUM += bb->TasksInside.size();
		
	}	
    tasksPerMachines = (float)tasksNUM / (float)numMachines;
	bool allPlaced = true;
    // if (prm->algType[1] == '1'){
    //     if (tasksNUM != Tasks->size() + TasksB->size()) {

    //         cout << "ERROR Tasks->size() + TasksB->size()\n";
    //         exit(0);
    //     }
    // }
    // else if(prm->algType[1] == '0'){
    //     if (tasksNUM != Tasks->size()) {
    //         cout << "ERROR Tasks->size() + TasksB->size()\n";
    //         exit(0);
    //     }
    // }
	//check if everything is placed
	
    
    if ((prm->algType[1] == '1')){
        for (size_t i = 0;i < Tasks->size();i++) {
            tt = Tasks->at(i);
            if (!tt->placed) {
                allPlaced = false;
            }
	    }
        for (size_t i = 0;i < TasksB->size();i++) {
            tt = TasksB->at(i);
            if (!tt->placed) {
                allPlaced = false;
            }
        }
        // if (!allPlaced) {
        //     std::cout << "ERROR -All tasks havent been placed\n";
        //     exit(0);
        // }
    }
		
}

bool DBP::needBalance(){
	//cout << "Mesa sto needBalance\n";
	for (size_t i = 0;i < this->UGlobal.size();i++) {
		//cout << UGlobal[i]->deleted << "| ";UGlobal[i]->showRectangle(" ~");
	}
	//cout << "\n";
	if (UGlobal.size() <= 1) {
		//cout << "\t\tNO NEED BALLANCE<=1\n";
		return false;
	}
	Rectangle *r1, *r2;
	r1 = r2 = NULL;
	sort(this->UGlobal.begin(), UGlobal.end(), sortRect);
	int j;
	bool find;
	Rectangle *curr=NULL;
	int shiftx;

	for (size_t i = 0;i < this->UGlobal.size() - 1;i++) {
		r1 = UGlobal[i];
		shiftx = r1->x2;
		find = false;
		if (!r1->deleted) {			
			j = i + 1;
			r2 = UGlobal[j];
			curr = r2;
			while (!r2->deleted && floatEquals(r1->y2, r2->y2) && shiftx==r2->x1) {
				find = true;
				for (size_t z = 0;z < r2->tasksInSect.size();z++) {
					r1->tasksInSect.push_back(r2->tasksInSect[z]);
				}
				j++;				
				r2->deleted = true;
				curr = r2;
				shiftx = curr->x2;
				if (j == this->UGlobal.size())
					break;
				r2 = UGlobal[j];
				
			}
			if (find) {
				curr->deleted = true;
				r1->x2 = curr->x2;
			}
		}		
	}
	
	return false;
}


float DBP::round(float d)
{

	float t = ceilf(d * 1000) / 1000;
	return t;
}


void DBP::createBinsHybrid(){
	// Category *cat = new Category(this->maxCap);
	// cat->id = 0;
	// Cats.push_back(cat);
	// Task *t;
	// Bin *b, *Binsert;
	// // for the small tasks
	// for (size_t i = 0;i < this->Tasks->size();i++) {
	// 	t = this->Tasks->at(i);
	// 	t->demand  = round(t->DBP_demand);
	// 	Cats[0]->insertTtoB_BF(t, '1');//'1' so as not to sort 
	// }
	// Bins = Cats[0]->Bins;
	// //large Tasks
	// for (size_t i = 0;i < TasksB->size();i++) {
	// 	t = TasksB->at(i);
	// 	b = new Bin();
	// 	b->TasksInside.push_back(t);
	// 	t->placed = true;
	// 	t->placedAt = t->DBP_demand;
	// 	Bins.push_back(b);
	// }

	Task *t;
	Bin *b, *Binsert;
	double maxCap_ = (double)this->maxCap;
	for (size_t i = 0;i < this->Tasks->size();i++){
		
		t = this->Tasks->at(i);
	//std::sort(this->Bins.begin(), this->Bins.end(), sortOpenTime);
	// balanceTime(t->start);
	if (Bins.empty()) {
		b = new Bin();
		//b->id = 0;
		b->freeCap_ = maxCap_- t->DBP_demand;
		b->start = t->start;//t->start tha htan to idio
		//b->end = t->end;
		//b->open = true;
		b->TasksInside.push_back(t);
		t->placed = true;
		this->Bins.push_back(b);
	}
	else {
		double min;
		Binsert = NULL;
		min = std::numeric_limits<double>::max();
		for (size_t i = 0;i < this->Bins.size();i++) {
			b = this->Bins[i];
			if (b->freeCap_ >= t->DBP_demand ) {
				if (b->freeCap_ - t->DBP_demand < min) {
					min = b->freeCap_ - t->DBP_demand;
					Binsert = b;
				}
			}
		}
		if (Binsert == NULL) {//den vrhka kanena prepei na ftiaksw ena
			b = new Bin();
			//	b->id = 0;
			b->freeCap_ = maxCap_ - t->DBP_demand;
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
			Binsert->freeCap_ -= t->DBP_demand;
		}
	}
	}
	for (size_t i = 0;i < TasksB->size();i++) {
		t = TasksB->at(i);
		b = new Bin();
		b->TasksInside.push_back(t);
		t->placed = true;
		t->placedAt = t->DBP_demand;
		Bins.push_back(b);
	}
	
}


