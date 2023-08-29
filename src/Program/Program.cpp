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
    if ((time.day > end.day) || ((time.day == end.day) && (time.hour > end.hour)) || ((time.day == end.day) && (time.hour == end.hour) && (time.minutes > end.minutes)) || ((time.day == end.day) && (time.hour == end.hour) && (time.minutes == end.minutes) && (time.seconds > end.seconds)))
    {
        return 0;
    }
    else if ((time.day < start.day) || ((time.day == start.day) && (time.hour < start.hour)) || ((time.day == start.day) && (time.hour == start.hour) && (time.minutes < start.minutes)) || ((time.day == start.day) && (time.hour == start.hour) && (time.minutes == start.minutes) && (time.seconds < start.seconds)))
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
    cJSON *program = cJSON_GetObjectItem(this->tuesday, "program");
    cJSON *event;
    cJSON_ArrayForEach(event, program)
    {
        RTC_TIME start;
        start.day = 5;
        start.hour = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeStart"), "hours")->valueint;
        start.minutes = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeStart"), "minutes")->valueint;
        RTC_TIME end;
        end.day = 5;
        end.hour = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeEnd"), "hours")->valueint;
        end.minutes = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeEnd"), "minutes")->valueint;
        if (isInInterval(time, start, end))
        {
            char time_str[50];
            sprintf(time_str, "%d%d:%d%d %s", start.hour / 10, start.hour % 10, start.minutes / 10, start.minutes % 10, cJSON_GetObjectItem(event, "text")->valuestring);
            events[countOfEvents++] = string(time_str) + string(" ") + cJSON_GetObjectItem(event, "text")->valuestring;
        }
        if (countOfEvents == 3)
        {
            return;
        }
    }

    program = cJSON_GetObjectItem(this->wednesday, "program");
    event;
    cJSON_ArrayForEach(event, program)
    {
        RTC_TIME start;
        start.day = 6;
        start.hour = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeStart"), "hours")->valueint;
        start.minutes = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeStart"), "minutes")->valueint;
        RTC_TIME end;
        end.day = 6;
        end.hour = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeEnd"), "hours")->valueint;
        end.minutes = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeEnd"), "minutes")->valueint;
        if (isInInterval(time, start, end))
        {
            char time_str[50];
            sprintf(time_str, "%d%d:%d%d %s", start.hour / 10, start.hour % 10, start.minutes / 10, start.minutes % 10, cJSON_GetObjectItem(event, "text")->valuestring);
            events[countOfEvents++] = string(time_str);
        }
        if (countOfEvents == 3)
        {
            return;
        }
    }
    program = cJSON_GetObjectItem(this->thursday, "program");
    event;
    cJSON_ArrayForEach(event, program)
    {
        RTC_TIME start;
        start.day = 7;
        start.hour = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeStart"), "hours")->valueint;
        start.minutes = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeStart"), "minutes")->valueint;
        RTC_TIME end;
        end.day = 7;
        end.hour = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeEnd"), "hours")->valueint;
        end.minutes = cJSON_GetObjectItem(cJSON_GetObjectItem(event, "timeEnd"), "minutes")->valueint;
        if (isInInterval(time, start, end))
        {
            char time_str[50];
            sprintf(time_str, "%d%d:%d%d %s", start.hour / 10, start.hour % 10, start.minutes / 10, start.minutes % 10, cJSON_GetObjectItem(event, "text")->valuestring);
            events[countOfEvents++] = string(time_str) + string(" ") + cJSON_GetObjectItem(event, "text")->valuestring;
        }
        if (countOfEvents == 3)
        {
            return;
        }
    }
}