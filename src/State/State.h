#ifndef __STATE_H__
#define __STATE_H__

#include "stdlib.h"
#include "string"

typedef enum state {
    START,
    STOP,
    RESET,
    NEW_DATA,
    NONE
} STATE;
using namespace std;
class State
{
private:
    string university_name;
    int time;
    STATE state;
public:
    State();
    ~State();
    void setState(STATE s);
    void setData(string str, int time);

    string getString();
    int getData();
    STATE getState();
};

#endif