#include "stdafx.h"
#include "Parameters.h"
#include <fstream>

void Parameters::createNewFile(){ //not sure if this needed rn
    int start, wait, dur, demand;
    ofstream myfile2;
    fstream myfile("data/" + this->dataFileName, std::ios_base::in);
    if(!myfile.is_open()){
        cout << "Error openeing file\n"; exit(0);
    }
    string fname = this->dataFileName;
    myfile2.open("data2/" + fname);
    int i=0, id;
    double q1;

    while(!myfile.eof()){
        myfile >> id; // don't understand what line line does
        myfile>> start>>wait>>dur>>demand; //don't understand what line line does
        for(int j=0; j<13; j++){
            myfile >> q1;
        }

        if (id== -1)
            myfile2 << start << "\t" << wait << "\t" << dur << "\t" << demand; //don't understand what line line does
        else
            myfile2 << start << "\t" << wait << "\t" << dur  << "\t" << demand << "\n"; //don't understand what line line does

        if(i % 10000==0)
            cout << i << "\n";
        i++;
        if(id == -1)
            break;
    }
    myfile2.flush();
    myfile2.flush();
    myfile.close();
    cout << "Ending\n";
    exit(0);
}

void Parameters::parseBig(){
    fstream myfile("data/vm_cpu_readings-file-1-of-125.cvs", std::ios_base::in);
    if (!myfile.is_open()){
            cout << "Error openeing file\n"; exit(0);
    }
    std::string line;
    while (std::getline(myfile, line)){
        cout << line << "\n";
        getchar();
    }
}

Parameters::Parameters(){
    binsOpened = 0;
    heteId = 0;
    maxMachinesParallel = 0;
    noOverlap = 0;
    algName=".";
    actualTasksNum = 0;
    algType = ".";
    thersholdValue = 0;
}

Parameters::~Parameters(){
    
}

void Parameters::findGlobalX()
{
    string fName;
    if (algType == "20") {
        fName = "actual_" + to_string(maxCapacity);
    }
    else if (algType == "21") {
        fName = "REM1_" + to_string(maxCapacity);
    }
    else {
        cout << "Cant run other algs\n";exit(0);
    }

    fstream myfile("thresholds/" + fName + ".txt", std::ios_base::in);
    if (!myfile.is_open()) {
        cout << "Error opening file\n" << fName << "\n";exit(0);
    }
        int fid;
        float thres;
        bool find = false;
        while (!myfile.eof()) {
            myfile >> fid >> thres;
            if (this->fileId == fid) {
                find = true;
                this->thersholdValue = thres;
                break;
            }
        }
        if (!find) {
            cout << "Couldn't find Fileid\n";exit(0);
        }
        cout << "THRESHOLD " << this->thersholdValue << "\n";
	
}