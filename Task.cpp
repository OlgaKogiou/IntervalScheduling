#include "stdafx.h"
#include "Task.h"


Task::Task(int s, int e){
    start=s;
    end=e;
    length=end-start;
    if (length <=0){
        cout << "invalid length\n";
        exit(0);
    }
    placed = false;
    placedAt = -1;
    machineId = -1;
    checkOnlineDPB = false;
    waitForExecution =0.0;
}

void Task:: showTask(){
    cout << id<<" ["  << start << "~" << end << ":" << length << "] FD "<<fakeD<<"\n";
}

Task::Task(){

}

Task::~Task(){
    cout<< "Delete" << this-> id << "\n";
    getchar();
}