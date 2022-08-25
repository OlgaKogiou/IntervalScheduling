#include "stdafx.h"
#include "Machine.h"

// KNAP
void Machine::createKnapsack()
{
	// sack = new Knapsack(prm);
	sack->id = this->id;
	sack->G = this->cap;
	sack->matrix = this->matrix;
	sack->objList = this->Edges;
	sack->sol = NULL;
}
//it messes with KSol
void Machine::placeTask(Task *t) {
	KnapsackInterval *temp = new KnapsackInterval();
	if (kSol != NULL) {//prepei na dw an tha xwraei
		temp->maxD = kSol->I->maxD;
		Point *pp;
		for (size_t i = 0;i < kSol->I->P.size();i++) {
			pp = new Point();
			pp->s = kSol->I->P[i]->s;
			pp->c = kSol->I->P[i]->c;
			pp->d = kSol->I->P[i]->d;
			temp->P.push_back(pp);
		}
		//mlkia poy to kanw etsi.??
		temp->addInterval(t);
		if (temp->maxD <= cap) {//xwraei kai th mpei!
							  //diagrafw ta palia stoixeia
			
			delete kSol->I;
			kSol->I = temp;
			//ti paizei me to sol->B
			kSol->S.push_back(t);

			t->machineId = this->id;
			t->placed = true;

			if (this->startTime == -1) {
				this->startTime = t->start;
			}
		}
		else {//dn xwraei ara diagrafw to temp
			
			delete temp;
			temp = NULL;
		}
	}
	else {//o sakos einai adeios check an xwraei
		if (t->fakeD <= cap) {//xwraei
			temp->maxD = t->fakeD;
			Point *pp = new  Point();
			pp->s = t->start;pp->c = t->end;pp->d = t->fakeD;
			temp->P.push_back(pp);
			kSol = new KnapsackArray();
			kSol->I = temp;
			kSol->B = 0.0;//sure???
			kSol->S.push_back(t);

			t->machineId = this->id;
			t->placed = true;
			if (this->startTime == -1) {
				this->startTime = t->start;
			}
		}
		else {
			t->showTask();
			cout << "place Task adeio machine kai dn xwraei..\n";
			exit(0);
			delete temp;
		}
	}
	//gia debug ston online
	//if (t->placed) {
	//	cout << "Placed at "<<this->id<<":      ";
	//	t->showTask();
	//	
	//}
}
// ~KNAP

// EDGES
int Machine::insertEdgeInMachine(Edge * e)
{
	Task *t1, *t2;

	t1 = e->n1;t2 = e->n2;
	if (t1->placed || t2->placed) {
		cout << "\t\t\tt1->placed || t2->placed";exit(0);
	}
	this->placeTask(t1);
	if (!t1->placed) {
		return -1;
	}
	this->placeTask(t2);
	if (!t2->placed) {
		this->removeTaskReturnBusyReal(t1);
		return -1;
	}
	this->busyTime();
	e->placed = true;
	return this->totalBusyTime;
}

int Machine::insertEdgeInMachineTemp(Edge * e)
{
	Task *t1, *t2;

	t1 = e->n1;t2 = e->n2;
	if (t1->placed || t2->placed) {
		cout << "\t\t\tt1->placed || t2->placed";exit(0);
	}
	this->placeTask(t1);
	if (!t1->placed) {
		return -1;
	}
	this->placeTask(t2);
	if (!t2->placed) {
		this->removeTaskReturnBusyReal(t1);
		return -1;
	}
	this->busyTime();
	int newB = this->totalBusyTime;
	this->removeTaskReturnBusyReal(t1);
	this->removeTaskReturnBusyReal(t2);
	return newB;
}
// ~EDGES


// returns how many tasks are inside the machine
// if the task doesn't fit it returns -1
int Machine::numberofTasksInside(Task *t)
{
	Machine *m = this;
	Point *pp;
	int maxD = -1;
	int mn, mx;
	for (size_t i = 0;i < m->kSol->I->P.size();i++) {
		pp = m->kSol->I->P[i];
		if (pp->s > t->end) {
			break;
		}
		mn = min(pp->c, t->end);
		mx = max(pp->s, t->start);
		if (mn > mx) {//exoume overlap
			if (pp->d > maxD) {
				maxD = pp->d;
			}
		}
	}
	if (this->kSol == NULL) {
		return 0;
	}
	if (maxD == -1) {
		return this->kSol->S.size();
	}

	if (this->cap - (t->fakeD + maxD) >= 0) {
		return this->kSol->S.size();
	}
	else {
		return -1;
	}	

}
int Machine::computeIntersection(Task * t)
{
	Machine *m = this;
	Point *pp;
	int mmax,mmin;
	int intesection = 0;
	for (size_t i = 0;i < m->kSol->I->P.size();i++) {
		pp = m->kSol->I->P[i];
		mmax = max(pp->s, t->start);
		mmin = min(pp->c, t->end);
		if (mmax < mmin ) {// overlap
			if (pp->d == t->fakeD) {

			}
			else {
				intesection += (pp->c - pp->s);
			}
		}
	}
	return intesection;
}
int Machine::addNewTaskReturnBusyTemp(Task * t)
{
	if (0) {
		this->kSol->I = new KnapsackInterval();
		this->kSol->I->maxD = 1;
		Point *p = new Point();
		p->s = 5;	p->c = 10;	p->d = 10;	this->kSol->I->P.push_back(p);
		p = new Point();
		p->s = 10;	p->c = 20;	p->d = 20;  this->kSol->I->P.push_back(p);
		p = new Point();
		p->s = 30;	p->c = 40;	p->d = 30;	this->kSol->I->P.push_back(p);
		p = new Point();
		p->s = 60;	p->c = 80;	p->d = 40;	this->kSol->I->P.push_back(p);
		t->start = 2;
		t->end = 60;
		t->fakeD = 6;
		this->kSol->I->showIntervals("\n");
		this->kSol->I->addInterval(t);
	//	this->kSol->I->addInterval(t);
		this->kSol->I->showIntervals("\n");
		cout << "MaxD " << this->kSol->I->maxD << "\n";
		exit(0);
		////////////////////////////////////////

	}
	Machine *m = this;
	KnapsackInterval *temp = new KnapsackInterval();
	temp->maxD = kSol->I->maxD;
	Point *pp;
	for (size_t i = 0;i < kSol->I->P.size();i++) {
		pp = new Point();
		pp->s = kSol->I->P[i]->s;
		pp->c = kSol->I->P[i]->c;
		pp->d = kSol->I->P[i]->d;
		temp->P.push_back(pp);
	}
	temp->addInterval(t);
	
	if (temp->maxD <= cap) {
		int bt = 0; // busy time of temp solution
		for (size_t j = 0;j < temp->P.size();j++) {
			pp = temp->P[j];
			bt += (pp->c - pp->s);
		}
		if (bt <= 0) {
			cout << "bt <= 0";
			exit(0);
		}
		delete temp;
		temp = NULL;
		return bt;
	}
	else {
		delete temp;
		temp = NULL;
		return -1;//den xwraei
	}
}
int Machine::removeTaskReturnBusyTemp(Task * t)
{
	Machine *m = this;
	Point *pp;
	int mmax,mmin;
	int newBusy = 0;
	for (size_t i = 0;i < m->kSol->I->P.size();i++) {
		pp = m->kSol->I->P[i];
		mmax = max(pp->s, t->start);
		mmin = min(pp->c, t->end);
		if (mmax < mmin ) {// overlap
			if (pp->d == t->fakeD) {

			}
			else {
				newBusy += (pp->c - pp->s);
			}
		}
		else {
			newBusy += (pp->c - pp->s);
		}
	}
	return newBusy;
}
void Machine::removeTaskReturnBusyReal(Task * t)
{
	
	Machine *m = this;
	if (m->kSol == NULL) {
		cout << "EINAI ADEIO\n";exit(0);
	}
	KnapsackInterval *temp = new KnapsackInterval();
	temp->maxD = kSol->I->maxD;
	Point *pp,*pnew;
	int mmin, mmax;
	
	bool find = false;
	int index=-1;
	for (size_t i = 0;i < m->kSol->S.size();i++) {
		if(t->id== m->kSol->S[i]->id){
			find = true;
			index = i;
		}
	}
	if (!find) {
		cout << "Den yparxei to task\n";
		exit(0);
	}
	else {
		m->kSol->S.erase(m->kSol->S.begin() + index);
	}
	for (size_t i = 0;i < m->kSol->I->P.size();i++) {
		pp = m->kSol->I->P[i];
		mmax = max(pp->s, t->start);
		mmin = min(pp->c, t->end);
		if (mmax < mmin ) {// overlap
			if (pp->d == t->fakeD) {
				
			}
			else {
				int dold = pp->d;
				dold -= t->fakeD;
				if (dold <= 0) {
					cout << "pp->d <= 0";exit(0);
				}
				pnew = new Point();
				pnew->s = pp->s;
				pnew->c = pp->c;
				pnew->d = dold;
				
				temp->P.push_back(pnew);
			}
		}
		else {
			pnew = new Point();
			pnew->s = pp->s;
			pnew->c = pp->c;
			pnew->d = pp->d;
			
			temp->P.push_back(pnew);
		}
	}
	int maxD1 = -1;
	for (size_t i = 0;i < temp->P.size();i++) {
		pp = temp->P[i];
		if (pp->d > maxD1)
			maxD1 = pp->d;
	}
	temp->maxD = maxD1;
	delete m->kSol->I;
	m->kSol->I = NULL;

	m->kSol->I = temp;
	this->busyTime();
	t->placed = false;
}
void Machine::removeTaskReturnBusyRealIterator(Task * t)
{

	Machine *m = this;
	if (m->kSol == NULL) {
		cout << "EINAI ADEIO\n";exit(0);
	}
	KnapsackInterval *temp = new KnapsackInterval();
	temp->maxD = kSol->I->maxD;
	Point *pp, *pnew;
	int mmin, mmax;

	bool find = false;
	int index = -1;
	for (size_t i = 0;i < m->kSol->S.size();i++) {
		if (t->id == m->kSol->S[i]->id) {
			find = true;
			index = i;
		}
	}
	if (!find) {
		cout << "Den yparxei to task\n";
		exit(0);
	}
	
	for (size_t i = 0;i < m->kSol->I->P.size();i++) {
		pp = m->kSol->I->P[i];
		mmax = max(pp->s, t->start);
		mmin = min(pp->c, t->end);
		if (mmax < mmin) {// overlap
			if (pp->d == t->fakeD) {

			}
			else {
				int dold = pp->d;
				dold -= t->fakeD;
				if (dold <= 0) {
					cout << "pp->d <= 0";exit(0);
				}
				pnew = new Point();
				pnew->s = pp->s;
				pnew->c = pp->c;
				pnew->d = dold;

				temp->P.push_back(pnew);
			}
		}
		else {
			pnew = new Point();
			pnew->s = pp->s;
			pnew->c = pp->c;
			pnew->d = pp->d;

			temp->P.push_back(pnew);
		}
	}
	int maxD1 = -1;
	for (size_t i = 0;i < temp->P.size();i++) {
		pp = temp->P[i];
		if (pp->d > maxD1)
			maxD1 = pp->d;
	}
	temp->maxD = maxD1;
	delete m->kSol->I;
	m->kSol->I = NULL;

	m->kSol->I = temp;
	this->busyTime();
	t->placed = false;
}
void Machine::addNewTaskReturnBusyReal(Task * t)
{
	//int bOld = this->totalBusyTime;

	t->machineId = this->id;
	this->kSol->I->addInterval(t);
	this->busyTime();
	this->kSol->S.push_back(t);
	if (this->kSol->I->maxD > cap) {
		cout << "this->kSol->I->maxD > cap";exit(0);
	}
	t->placed = true;
}

// Constructors
Machine::Machine()
{
	numTFromPrevZ = 0;
	currPoint = 0;
	cout << "Machine()";
	exit(0);
}

Machine::Machine(int id, int cap, vector<Edge*>* Edges, float ** matrix, Parameters *p)
{
	startTime = -1;
	string smallLarge = "";

	onlineLastCheck = -1;
	closed = false;
	//this->algType = type;
	this->prm = p;
	this->id = id;
	this->cap = cap;
	this->Edges = Edges;
	this->matrix = matrix;
	this->kSol = NULL;
	this->sack = NULL;
	numTFromPrevZ = 0;
	tottalTasksInside = 0;
	this->totalBusyTimeOnline = 0;
	occSpace = 0;
	currPoint = 0;


	this->minDur = 0;
	this->maxDur = 0;

	//hete....
	if (prm->hete > 0) {
		this->cap = prm->heteCores[prm->heteId];
		prm->heteId++;
	}
		
	
}

Machine::~Machine()
{//thn eftiaksa mono gia to romoveKnapsack
	//opote dn kanw delete knapsack edw mesa
	//cout << "~Machine()\n"; getchar();
	if (this->kSol != NULL) {
		delete this->kSol;
	}
	
}
//~Constructors

// OTHER METHODS
int Machine::busyTime()
{
	//cout << "BT " << this->objNo<<"\n";
	totalBusyTime = 0;
	Point *p;
		if (!this->kSol->I->P.empty()) {
			for (size_t j = 0;j < this->kSol->I->P.size();j++) {
				p = this->kSol->I->P[j];
				totalBusyTime += (p->c - p->s);
			}
		}
	return totalBusyTime;
}

void Machine::finndFreeSpace(int zoneWidth)
{
	Point *ki;
	int occ=0;
	for (size_t i = 0;i < kSol->I->P.size();i++) {
		ki = kSol->I->P[i];
		occ += (ki->c - ki->s);
	}

	this->freeSpace= zoneWidth-occ;
}

void Machine::finndOccSpace()
{
	
	if (kSol == NULL) {
		occSpace = 0;
	}
	else {
		Point *ki;
		for (size_t i = 0;i < kSol->I->P.size();i++) {
			ki = kSol->I->P[i];
			occSpace += (ki->c - ki->s);
		}
	}	
}
// ~OTHER METHODS