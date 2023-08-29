#include "RTC.h"

RTC::RTC()
{
}

RTC::~RTC()
{
}

RTC_TIME RTC::getActualTime()
{
    return this->time;
}
void RTC::sync(RTC_TIME time)
{
    this->time = time;
}

void RTC::incrementTime()
{
    changedTime = true;
    if (this->time.seconds < 59)
    {
        this->time.seconds++;
        changedTime = false;
    }
    else if (this->time.minutes < 59)
    {
        this->time.minutes++;
        this->time.seconds = 0;
    }
    else if (this->time.hour < 23)
    {
        this->time.hour++;
        this->time.minutes = 0;
        this->time.seconds = 0;
    }
    else
    {
        this->time.day++;
        this->time.hour = 0;
        this->time.minutes = 0;
    }
}

bool RTC::getChange()
{
    return changedTime;
}

void RTC::removeChange()
{
    changedTime = false;
}