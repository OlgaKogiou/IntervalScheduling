#pragma once
#include "Task.h"
#include "Bin.h"
#include "Category.h"
#include "Parameters.h"
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
class DBPOnline
{
public:
   Parameters *prm; 
	long long int totalBusyTime;
	float tasksPerMachines;
	string StringType;
	vector <int> allPoints;
	int clockDBP, r, maxCap, numMachines;
	float a;
	vector <Task *> *Tasks;
    vector <Category *> Cats;
	void createCats();
	void placement();
	void findResults();
	void originalFirstFit();
	void originalBestFit();

	DBPOnline(vector<Task *> *, string t,int);
	DBPOnline(Parameters *p);
	DBPOnline();
	~DBPOnline(); 
};
