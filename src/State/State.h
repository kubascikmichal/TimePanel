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
    BUZZER_ON,
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
    bool mode;
    bool wasChange;
public:
    State();
    ~State();
    void setState(STATE s);
    void setData(string str, int time);
    void setBrightness(int brightness);
    void setMode(bool mode);

    string getString();
    bool getMode();
    bool getChange();
    void resetChange();
    int getData();
    int getBrightness();
    STATE getState();
};

#endif