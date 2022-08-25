#include "Task.h"
#include "DBP.h"
#include "Bin.h"
#include "Task.h"
#include "Parameters.h"
#include "DBPOnline.h"
#include "TSUSC.h"
#include "stdafx.h"

class Scheduler
{
public:
    DBP *dbp = NULL;
    DBPOnline *dbpOnline;
    void initDBP(string);
    void ParseTasksDBP();
    void parseTasks();
    void initDBPOnline(string);
    void initOnline(string);
    void initTsusc(string);
    TSUSC *tsusc;

    Parameters *prm;
    float round(float);
    Scheduler();
    Scheduler(Parameters *p);
    
    ~Scheduler();
};