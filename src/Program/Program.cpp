#include "Program.h"

Program::Program()
{
    this->loadProgram();
}

Program::~Program()
{
}

void Program::loadProgram()
{
    extern const unsigned char tuesday_start[] asm("_binary_src_Files_Tuesday_json_start");
    extern const unsigned char tuesday_end[] asm("_binary_src_Files_Tuesday_json_end");
    const size_t tuesday_size = (tuesday_end - tuesday_start);
    this->tuesday = cJSON_Parse((const char *)tuesday_start);
    // printf("%s\n\r", cJSON_PrintUnformatted(this->tuesday));

    extern const unsigned char wednesday_start[] asm("_binary_src_Files_Wednesday_json_start");
    extern const unsigned char wednesday_end[] asm("_binary_src_Files_Wednesday_json_end");
    const size_t wednesday_size = (wednesday_end - wednesday_start);
    this->wednesday = cJSON_Parse((const char *)wednesday_start);
    // printf("%s\n\r", cJSON_PrintUnformatted(this->wednesday));

    extern const unsigned char thursday_start[] asm("_binary_src_Files_Thursday_json_start");
    extern const unsigned char thursday_end[] asm("_binary_src_Files_Thursday_json_end");
    const size_t thursday_size = (thursday_end - thursday_start);
    this->thursday = cJSON_Parse((const char *)thursday_start);
    // printf("%s\n\r", cJSON_PrintUnformatted(this->thursday));
}

int Program::isInInterval(RTC_TIME time, RTC_TIME start, RTC_TIME end)
{
    if ((time.day > end.day) ||
        ((time.day == end.day) && (time.hour > end.hour)) ||
        ((time.day == end.day) && (time.hour == end.hour) && (time.minutes > end.minutes)) ||
        ((time.day == end.day) && (time.hour == end.hour) && (time.minutes == end.minutes) && (time.seconds > end.seconds)))
    {
        return 0;
    }
    else if ((time.day < start.day) ||
             ((time.day == start.day) && (time.hour < start.hour)) ||
             ((time.day == start.day) && (time.hour == start.hour) && (time.minutes < start.minutes)) || 
             ((time.day == start.day) && (time.hour == start.hour) && (time.minutes == start.minutes) && (time.seconds < start.seconds)))
    {
        return 2;
    }
    else
    {
        return 1;
    }
}

void Program::getUpcommingEvents(RTC_TIME time, string *events)
{
    int countOfEvents = 0;
    for (int i = 0; i < 3; i++)
    {
        events[i] = string("");
    }

    countOfEvents = process(this->tuesday, events, countOfEvents, time);
    if (countOfEvents != 3)
    {
        countOfEvents = process(this->wednesday, events, countOfEvents, time);
    }
    if (countOfEvents != 3)
    {
        countOfEvents = process(this->thursday, events, countOfEvents, time);
    }
    return;
}

int Program::process(cJSON *day, string *events, int counter, RTC_TIME time)
{
    cJSON *program = cJSON_GetObjectItem(day, "program");
    cJSON *event;
    cJSON_ArrayForEach(event, program)
    {
        RTC_TIME start;
        start.day = cJSON_GetObjectItem(day, "day")->valueint;
        start.hour = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeStart"), "hours")->valueint;
        start.minutes = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeStart"), "minutes")->valueint;
        RTC_TIME end;
        end.day = cJSON_GetObjectItem(day, "day")->valueint;
        end.hour = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeEnd"), "hours")->valueint;
        end.minutes = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeEnd"), "minutes")->valueint;
        int ret = isInInterval(time, start, end);
        if (ret)
        {
            char c = (ret == 2) ? 'U' : 'A';
            char time_str[100];
            sprintf(time_str, "%c%d%d:%d%d           %s",c, start.hour / 10, start.hour % 10, start.minutes / 10, start.minutes % 10, cJSON_GetObjectItem(event, "text")->valuestring);
            events[counter++] = string(time_str);
        }
        if (counter == 3)
        {
            return counter;
        }
    }
    return counter;
}