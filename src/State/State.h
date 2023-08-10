#ifndef __STATE_H__
#define __STATE_H__

#include "stdlib.h"
#include "string"

typedef enum state {
    START,
    STOP,
    RESET,
    NEW_DATA,
    NEW_BRIGHTNESS,
    NONE
} STATE;
using namespace std;
class State
{
private:
    string university_name;
    int time;
    int brightness = 10;
    STATE state;
public:
    State();
    ~State();
    void setState(STATE s);
    void setData(string str, int time);
    void setBrightness(int brightness);

    string getString();
    int getData();
    int getBrightness();
    STATE getState();
};

#endif