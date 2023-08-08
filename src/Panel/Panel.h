#ifndef __PANEL_H__
#define __PANEL_H__

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "../Fonts/dotmat10pt7b_v2.h"
#include "../State/State.h"

#define PANEL_W 16
#define PANEL_H 16
#define PANEL_COUNT PANEL_H *PANEL_W

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
    int time;
    bool started;
    State *st;
    SemaphoreHandle_t mutex;

    void init();
    void changeTime();

public:
    Panel();
    ~Panel();
    void setup(State *st, SemaphoreHandle_t mut);
    void start();
    void stop();
    void reset();
    void setString(string str);
    static uint16_t ledInPanel(uint16_t x, uint16_t y, bool odd);
    static uint16_t myRemapFn(uint16_t x, uint16_t y);

    void run();
};

#endif