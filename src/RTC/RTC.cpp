#include "RTC.h"

RTC::RTC()
{
    changedTime = true;
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
    changedTime = true;
}

void RTC::incrementTime()
{

    if (this->time.seconds < 59)
    {
        this->time.seconds++;
    }
    else if (this->time.minutes < 59)
    {
        this->time.minutes++;
        this->time.seconds = 0;
        changedTime = true;
    }
    else if (this->time.hour < 23)
    {
        this->time.hour++;
        this->time.minutes = 0;
        this->time.seconds = 0;
        changedTime = true;
    }
    else
    {
        this->time.day++;
        this->time.hour = 0;
        this->time.minutes = 0;
        changedTime = true;
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
