#include <WiFi.h>
#include "HTTP_SERVER/HTTP_Server.h"

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "Fonts/dotmat10pt7b_v2.h"

const char *ssid = "ESP32";        // Enter SSID here
const char *password = "12345678"; // Enter Password here
#define PANEL_W 16
#define PANEL_H 16
#define PANEL_COUNT PANEL_H *PANEL_W

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(64, 32, 15);

uint16_t ledInPanel(uint16_t x, uint16_t y, bool odd)
{
    if (!odd)
        return 16 * (x) + ((x % 2) ? 16 - y - 1 : y);
    else
        return 16 * (16 - x - 1) + ((x % 2) ? 16 - y - 1 : y);
}

uint16_t myRemapFn(uint16_t x, uint16_t y)
{
    int panel_X = x / PANEL_W;
    int panel_Y = y / PANEL_H;
    int base_matrix[2][4] = {{0, 3, 4, 7},
                             {1, 2, 5, 6}};

    return PANEL_COUNT * base_matrix[panel_Y][panel_X] + ledInPanel(x % PANEL_W, y % PANEL_H, panel_X % 2);
}

void setup()
{
    Serial.begin(115200);

    WiFi.softAP(ssid, password);
    HTTP_Server *server = new HTTP_Server();
    Serial.println("All set");

    matrix.setRemapFunction(myRemapFn);
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(20); // jas
    matrix.setFont(&dotmat10pt7b_v2);
    matrix.fillScreen(0);
    matrix.setTextColor(matrix.Color(0, 0, 255));

    matrix.setCursor(2, 18);
    matrix.print(F("pozor auto"));
    matrix.show();
}

// TODO: Component to handle clocks and strings on panel. Add statuses.

void loop()
{
}