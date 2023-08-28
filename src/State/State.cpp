#include "State.h"
State::State()
{
    this->time = 0;
}
State::~State()
{
}
void State::setState(STATE s)
{
    this->state = s;
}
void State::setData(string str, int time)
{
    this->university_name = str;
    this->time = time;
    setState(NEW_DATA);
}

string State::getString()
{
    return university_name;
}
int State::getData()
{
    return time;
}
STATE State::getState()
{
    return state;
}

void State::setBrightness(int brightness)
{
    this->setState(NEW_BRIGHTNESS);
    this->brightness = brightness;
}

int State::getBrightness()
{
    return this->brightness;
}

void State::setMode(bool mode){
    this->mode = mode;
}

bool State::getMode(){
    return this->mode;
}