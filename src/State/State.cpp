#include "State.h"
State::State()
{
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