#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "cJSON.h"
#include "../RTC/RTC.h"
#include "string"

using namespace std;

class Program
{
private:
    cJSON *tuesday;
    cJSON *wednesday;
    cJSON *thursday;
    void loadProgram();
    int isInInterval(RTC_TIME time, RTC_TIME start, RTC_TIME end);
    int process(cJSON *day, string *events, int counter, RTC_TIME time);

public:
    Program();
    ~Program();
    void getUpcommingEvents(RTC_TIME time, string *events);
};
#endif