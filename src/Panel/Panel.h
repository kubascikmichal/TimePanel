#ifndef __PANEL_H__
#define __PANEL_H__

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "../Fonts/dotmat10pt7b_v2.h"

#define PANEL_W 16
#define PANEL_H 16
#define PANEL_COUNT PANEL_H *PANEL_W

using namespace std;

typedef struct time
{
    int minutes = 0;
    int seconds = 0;
} TIME;

class Panel
{
private:
    Adafruit_NeoMatrix *matrix;
    TIME actualTime;
    string university;

    void init();

public:
    Panel();
    ~Panel();
    void start();
    void stop();
    void reset();
    void setString(string str);
    static uint16_t ledInPanel(uint16_t x, uint16_t y, bool odd);
    static uint16_t myRemapFn(uint16_t x, uint16_t y);
};

#endif