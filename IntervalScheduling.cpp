#include "stdafx.h"
#include "Bin.h"
#include "Task.h"
#include "Parameters.h"
#include "Scheduler.h"

vector<string> dataSets;

void writeResults(Parameters *p, Scheduler *sc, long long int bTime, int nMach, float tasksPerMach, double exTime){
    ofstream myfile;
    int fid = p->fileId;
    string sfid;
    if(fid <=9){
        sfid="0"+to_string(p->fileId);
    }
    else{
        sfid=to_string(p->fileId);
    }

// only for FF average machine size
	float moMS=0.0;
	if (1) {
		
		int sum = 0;
		for (int i = 0;i < p->machinesPerTimeSlot.size();i++) {
			sum += p->machinesPerTimeSlot[i];
		}
		moMS = (float)sum / (float)p->machinesPerTimeSlot.size();

	}
	string tsusc = "";

    if (0) {
		cout << "if(0) Int Scheduling.cpp";
		p->algName += "_H" + to_string(p->hete);
		string capdem = "C" + to_string(p->maxCapacity) + "-D" + to_string(p->maxDemand) + "_";
		if (p->launchCostInit != 0) {
			capdem += to_string(p->launchCostInit) + "_";
		}
		if (0) {
			myfile.open("results/" + capdem + p->algName + "_results.txt", std::ios_base::app);

		}
		else {
			myfile.open("results/alltogehter.txt", std::ios_base::app);

		}
		myfile << p->algName << "\t"
			<< "f" + sfid << "\t"
			<< bTime << "\t"
			<< p->maxCapacity << "\t"
			<< p->actualTasksNum << "\t"
			<< p->maxCapacity << "\t"
			<< p->maxDemand << "\t"
			<< bTime << "\t"
			<< nMach << "\t"
			<< tasksPerMach << "\t"
			<< exTime << "\t"
			<< p->maxMachinesParallel << "\t"
			<< p->launchCostInit << "\n";


	}

	double sum = 0.0;
	for (int i = 0;i < p->Tasks.size();i++) {
		Task *t = p->Tasks[i];
		sum += t->waitForExecution;
	}
	sum += sum / (double)p->Tasks.size();
	if (0) {
		cout << "if(0) Int Scheduling.cpp  11";
		myfile.open("results/alltogether.txt", std::ios_base::app);

		//myfile << p->_algName << "\t"
		myfile << "f" + sfid << "\t"
			<< p->maxCapacity << "\t"
			<< bTime << "\t"
			<< exTime << "\t"
			<< sum << "\t"
			<< p->binsOpened << "\n";

	}

	if (1) {

		//myfile << p->_algName << "\t"
		myfile << "f" + sfid << "\t"
			<<p->dataFileName<<"\t"
			<< p->maxCapacity << "\t"
			<< p->Tasks.size() << "\t"
			<< exTime<<"\t"
			<< bTime << "\n";
		myfile.open("results.txt", std::ios_base::app);

	}
}
void addDataSets() {
	dataSets.push_back("0_NASA-iPSC-1993-3.1-cln.swf");
	dataSets.push_back("1_KTH-SP2-1996-2.1-cln.swf");
	dataSets.push_back("2_SDSC-Par-1996-3.1-cln.swf");
	dataSets.push_back("3_OSC-Clust-2000-3.1-cln.swf");
	dataSets.push_back("4_LLNL-Atlas-2006-2.1-cln.swf");
	dataSets.push_back("5_SDSC-Par-1995-3.1-cln.swf");
	dataSets.push_back("6_SDSC-SP2-1998-4.2-cln.swf");
	dataSets.push_back("7_CTC-SP2-1996-3.1-cln.swf");
	dataSets.push_back("8_Sandia-Ross-2001-1.1-cln.swf");
	dataSets.push_back("9_SDSC-DS-2004-2.1-cln.swf");
	dataSets.push_back("10_HPC2N-2002-2.2-cln.swf");
	dataSets.push_back("11_LPC-EGEE-2004-1.2-cln.swf");
	dataSets.push_back("12_SDSC-BLUE-2000-4.2-cln.swf");

	dataSets.push_back("13_grid5000_clean_trace.log");
	dataSets.push_back("14_GWA-T-4-AuverGrid.gwf");
	dataSets.push_back("15_GWA-T-11-LCG.gwf"); 
	dataSets.push_back("16_meta1-6-2013.swf");
	dataSets.push_back("17_wagap1-10-2013.swf");
	dataSets.push_back("18_eucalyptus_1.txt");
	dataSets.push_back("19_vmTable_Azure.txt");
	dataSets.push_back("20_KIT-FH2-2016-1.swf");
	dataSets.push_back("21_UniLu-Gaia-2014-2.swf");
	dataSets.push_back("22_PIK-IPLEX-2009-1.swf");
	dataSets.push_back("23_RICC-2010-2.swf");
	dataSets.push_back("pipes.txt");


}

int main(int argc, char* argv[]){
    addDataSets();
    string sfid; // for the file id to make it run
    srand(100);

    Parameters *prm = new Parameters();
    Scheduler *sc = new Scheduler(prm);
    // std::numeric_limits<float>::epsilon(); // not sure what it does and if needed
    clock_t begin = clock();
    // initialization
    std::string::size_type sz; //alias of size_t
    prm->fileId = std::stoi(argv[1], &sz);
    prm->dataFileName = dataSets[prm->fileId];
    prm->algType = argv[2];
    prm->maxTasksNum = std::stoi(argv[3], &sz);
    prm->maxCapacity = std::stoi(argv[4], &sz);
    prm->maxDemand = std::stoi(argv[5], &sz);

	if (prm->algType == "3_204_4") {
		prm->launchCostInit = prm->launchCost;
	}
	else {// launch cost for update machine
		prm->launchCostInit = prm->launchCost;
		prm->launchCost = 0.0;
	}
    if (prm->fileId <=9){
        sfid = "0" + to_string(prm->fileId);
    }
    else{
        sfid = to_string(prm->fileId);
    }

    if(prm->algType[0]=='0'){
        sc->initDBP(prm->algType);
		
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        sc-> dbp->findResults();
        cout << "------------------------------------------------\n";
        cout << "Algorithm " << prm->algName << "\n";
        cout << "Busy Time " << sc->dbp->totalBusyTime << "\n";
        cout << "Num Machines " << sc->dbp->numMachines << " | ";
        cout << "Tasks per Machines " << sc->dbp->tasksPerMachines << "\n";
		cout << "------------------------------------------------\n";
        writeResults(prm, sc, sc->dbp->totalBusyTime, sc->dbp->numMachines, sc->dbp->tasksPerMachines, prm->elapsed_secs);
    }
	else if (prm->algType[0]=='1')
	{
		sc->initDBPOnline(prm->algType);
		sc->dbpOnline->findResults();
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		cout << "------------------------------------------\n";
		cout << "Algorithm " << prm->algName << "\n";
		cout << "Busy Time " << sc->dbpOnline->totalBusyTime << "\n";
		cout << "Num Machines " << sc->dbpOnline->numMachines << " | ";
		cout << "Tasks per Machines " << sc->dbpOnline->tasksPerMachines << "\n";
		cout << "------------------------------------------\n";
		writeResults(prm, sc, sc->dbpOnline->totalBusyTime, sc->dbpOnline->numMachines, sc->dbpOnline->tasksPerMachines, prm->elapsed_secs);
		
	}
	else if (prm->algType[0] == '2') {
		sc->initTsusc(prm->algType);

		clock_t end = clock();
		sc->tsusc->findResults();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		cout << "------------------------------------------\n";
		cout << "Algorithm " << prm->algName << "\n";
		cout << "Busy Time " << sc->tsusc->totalBusyTime << "\n";
		cout << "Num Machines " << sc->tsusc->numMachines << " | ";
		cout << "Tasks per Machines " << sc->tsusc->tasksPerMachines << "\n";
		cout << "------------------------------------------\n";
		writeResults(prm, sc, sc->tsusc->totalBusyTime, sc->tsusc->numMachines, sc->tsusc->tasksPerMachines, prm->elapsed_secs);
	}
}