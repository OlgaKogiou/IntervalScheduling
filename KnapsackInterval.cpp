#include "stdafx.h"
#include "KnapsackInterval.h"
bool sortPoints(const Point *i, const Point *j) { if (i->s != j->s)return (i->s < j->s); else { return i->c < j->c; } }//small to large

void KnapsackInterval::copyIntervals(KnapsackInterval * in)
{
	for (size_t i = 0;i < this->P.size();i++) {
		delete this->P[i];
		this->P[i] = NULL;
	}
	this->P.clear();
	vector<Point *>().swap(this->P);

	this->maxD = 0;
	if (in->P.empty()) {
		//this->P.empty();
	}
	else {
		Point *po;
		this->maxD = in->maxD;
		for (size_t i = 0;i < in->P.size();i++) {
			po = new Point();
			po->s = in->P[i]->s;
			po->c = in->P[i]->c;
			po->d = in->P[i]->d;
			this->P.push_back(po);
		}
	}
}

void KnapsackInterval::addInterval(Task * n)
{
	
	KnapsackInterval * IS = this;
	//int preBT = 0;
	//for (size_t i = 0;i < IS->P.size();i++) {
	//	preBT += (IS->P[i]->c - IS->P[i]->s);
	//}
	//n->showTask();
	//IS->showIntervals(",");
	Point *Ia = new Point();
	vector <Point *> BC;
	Ia->s = n->start;Ia->c = n->end;Ia->d = n->fakeD;
	if (IS->P.empty()) {
		IS->P.push_back(Ia);
		IS->maxD = Ia->d;
		goto endInterval;
	}
	else {
		bool oloAr, oloDek;
		oloAr = false;oloDek = false;
		Point *first, *last;
		first = IS->P[0];
		last = IS->P[IS->P.size() - 1];
		if (first->s >= Ia->c) {
			oloAr = true;
			std::vector<Point *>::iterator it1;
			it1 = IS->P.begin();
			it1 = IS->P.insert(it1, Ia);
		}
		if (last->c <= Ia->s) {
			oloDek = true;
			IS->P.push_back(Ia);			
		}
		if (oloAr || oloDek) {	
			if (Ia->d > IS->maxD)
				IS->maxD = Ia->d;
			goto endInterval;
		}else{//den einai oute aristera oute dekia. tote kapou einai mesa
			bool overlap = false;// true an kanei estw kai ena overal
			Point *Iz;
			int checkIndex = -1;
			vector< Point * >::iterator it = IS->P.begin();
			while (it != IS->P.end()) {
				//checkIndex = i;
				//Iz = IS->P[i];
				Iz = *it;
				
				int Max, Min;
				Max = max(Ia->s, Iz->s);Min = min(Ia->c, Iz->c);
				if (Min > Max) {//intersect
					overlap = true;
					bool ar, arM, same,insd;
					ar = arM = same =insd= false;
					if (Ia->c <= Iz->s)
						ar = true;
					if (Ia->c <= Iz->c)
						arM = true;
					if (Iz->s == Ia->s&&Ia->c == Iz->c)
						same = true;
					if (Iz->s <= Ia->s&&Ia->c <= Iz->c)
						insd = true;
					Point *p1, *p2, *p3;
					p1 = new Point();p2 = new Point();p3 = new Point();
					p1->s = min(Ia->s, Iz->s);p1->c = max(Ia->s, Iz->s);
					p2->s = p1->c;p2->c = min(Ia->c, Iz->c);
					p3->s = p2->c;p3->c = max(Ia->c, Iz->c);
					if (ar) {
						delete p1;p1 = NULL;delete p2;p2 = NULL;delete p3;p3 = NULL;
						break;
						//tha bei sto telos afou den tha exei allaksei to Ia katholou
					}
					if (arM&&!insd&&!same) {						
						BC.push_back(p1); p1->d = Ia->d;
						BC.push_back(p2); p2->d = Ia->d + Iz->d;
						if (p3->s == p3->c) {
							delete p3;p3 = NULL;
						}else {
							BC.push_back(p3); p3->d = Iz->d;
						}
						it = IS->P.erase(it);
						delete Iz;Iz = NULL;
						delete Ia;Ia = NULL;
						goto endInterval;
					}
					if (same) {
						Iz->d = Ia->d + Iz->d;
						delete p1;		p1 = NULL;				delete p2;p2 = NULL;						delete p3;p3 = NULL;
						delete Ia;Ia = NULL;
						goto endInterval;
					}
					if (insd) {
						if (p1->s == p1->c) {//einai mazi apo to start
							BC.push_back(p2); p2->d = Ia->d + Iz->d;
							BC.push_back(p3); p3->d = Iz->d;
							delete p1;p1 = NULL;
							it = IS->P.erase(it);
							delete Iz;Iz = NULL;
							delete Ia;Ia = NULL;
							goto endInterval;
						}
						if (p3->s == p3->c) {//einai mazi apo sto end
							BC.push_back(p1); p1->d = Iz->d;
							BC.push_back(p2); p2->d = Ia->d + Iz->d;
							delete p3;p3 = NULL;
							it = IS->P.erase(it);
							delete Iz;Iz = NULL;
							delete Ia;Ia = NULL;
							goto endInterval;
						}
					
						BC.push_back(p1); p1->d = Iz->d;
						BC.push_back(p2); p2->d = Iz->d+Ia->d;
						BC.push_back(p3); p3->d = Iz->d;
						it = IS->P.erase(it);
						delete Iz;Iz = NULL;
						delete Ia;Ia = NULL;
						goto endInterval;
					}
					//vrisketai kapou deksia (den mporei ektos giati alliws dn tha ekane overlap
					if (p1->s == p1->c) {
						Ia->s = Iz->c;
						delete p1;p1 = NULL;
						Iz->s = p2->s;Iz->c = p2->c;
						Iz->d = Iz->d + Ia->d;		
						delete p2;p2 = NULL;
						delete p3;p3 = NULL;
					}
					else if(Ia->s<Iz->s){//to Ia einai megaluytero kai apo tis 2 pleures tou Iz
						Ia->s = Iz->c;
						BC.push_back(p1);p1->d = Ia -> d;

						Iz->s = p2->s;Iz->c = p2->c;Iz->d = Iz->d+Ia->d;
						delete p2; p2 = NULL;
						delete p3; p3 = NULL;
					}
					else {
						Ia->s = Iz->c;
						Iz->s = p1->s;Iz->c = p1->c;Iz->d = Iz->d;
						delete p1; p1 = NULL;
						BC.push_back(p2);p2->d = Iz->d + Ia->d;
						//BC.push_back(p3);p3->d = Ia->d;
						delete p3;p3 = NULL;
					
					}
					++it;
				}
				else {
					++it;
				}
			}
		}
	}
	//prepei na ginoun sort
	if (Ia->s == Ia->c) {
		cout << "Ia->s == Ia->d";
		exit(0);
	}
	IS->P.push_back(Ia);
endInterval:
	cout << "";
	for (size_t i = 0;i < BC.size();i++) {
		IS->P.push_back(BC[i]);
	}
	int maxD = -1;
	for (size_t i = 0;i < IS->P.size();i++) {
		if (IS->P[i]->d > maxD)
			maxD = IS->P[i]->d;
	}
	IS->maxD = maxD;
	std::sort(IS->P.begin(), IS->P.end(), sortPoints);
	BC.clear();
	vector<Point *>().swap(BC);
	cout << "";

}

void KnapsackInterval::showIntervals(string s)
{
	Point *pp;
	for (size_t i = 0;i < P.size();i++) {
		pp = P[i];
		cout << "[" << pp->s << "," << pp->c << "](" << pp->d << ")"<<s;
	}
	cout << "\n";
}

KnapsackInterval::KnapsackInterval()
{
}

KnapsackInterval::~KnapsackInterval(){
	//cout << "Delete Interval\n";
	for (size_t z = 0;z < this->P.size();z++) {
		delete this->P[z];
		this->P[z] = NULL;
	}
	this->P.clear();
	vector<Point *>().swap(this->P);
}

bool KnapsackInterval::floatEquals(float a, float b)
{
	return fabs(a - b) < std::numeric_limits<float>::epsilon();
}
