#ifndef __PANEL_H__
#define __PANEL_H__

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "../Fonts/dotmat10pt7b_v2.h"
#include "../State/State.h"
#include "../RTC/RTC.h"

#define PANEL_W 16
#define PANEL_H 16
#define PANEL_COUNT PANEL_H *PANEL_W

#define DELAY_TIME 1000 // in ms
#define DELAY_STRING 20 // in ms -  change by 1px/30ms = 64px/2s

#define LOWER_CASE 8
#define UPPER_CASE 12
#define WHITESPACE 3
#define NUMBER 11
#define ONE 7
#define COLON 6

using namespace std;

typedef struct time
{
    int minutes = 0;
    int seconds = 0;
    bool decr = true;
} TIME;

class Panel
{
private:
    Adafruit_NeoMatrix *matrix;
    TIME actualTime;
    string university;
    int size;
    int time;
    bool started;
    State *st;
    SemaphoreHandle_t mutex;
    TaskHandle_t handle;

    RTC *rtc;
    SemaphoreHandle_t rtcMutex;

    uint32_t lastTimeChange;
    uint32_t lastStringChange;
    bool isChange = false;
    int index = 0;
    void init();
    void changeTime();
    int len(const char *str, int length);
    int timerOffset(const char *str, int length);

public:
    Panel();
    ~Panel();
    void setup(State *st, SemaphoreHandle_t mut, TaskHandle_t handle);
    void setRTC(RTC *rtc, SemaphoreHandle_t mut);
    void start();
    void stop();
    void reset();
    void setString(string str);
    static uint16_t ledInPanel(uint16_t x, uint16_t y, bool odd);
    static uint16_t myRemapFn(uint16_t x, uint16_t y);

    void run();
};

#endif