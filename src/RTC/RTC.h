#ifndef __RTC_H__
#define __RTC_H__
#include <stdint.h>

typedef struct rtc_time
{
    uint8_t day = 0;
    uint8_t hour = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;
} RTC_TIME;

class RTC
{
private:
    RTC_TIME time;
public:
    RTC();
    ~RTC();
    RTC_TIME getActualTime();
    void sync(RTC_TIME time);
    void incrementTime();
};

#endif