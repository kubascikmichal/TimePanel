#include <WiFi.h>
#include "HTTP_SERVER/HTTP_Server.h"
#include "Panel/Panel.h"

const char *ssid = "ESP32";        // Enter SSID here
const char *password = "12345678"; // Enter Password here


void setup()
{
    Serial.begin(115200);

    WiFi.softAP(ssid, password);
    HTTP_Server *server = new HTTP_Server();
    Serial.println("All set");
    Panel* p = new Panel();
}


void loop()
{
}