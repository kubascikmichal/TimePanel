#include <WiFi.h>
#include "HTTP_SERVER/HTTP_Server.h"
#include "Panel/Panel.h"
#include "State/State.h"

const char *ssid = "Panel";        // Enter SSID here
const char *password = "12345678"; // Enter Password here


void setup()
{
    Serial.begin(115200);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    WiFi.softAP(ssid, password);
    HTTP_Server *server = new HTTP_Server();
    SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
    State* s = new State();
    server->setup(s, mutex);
    Panel* p = new Panel();
    p->setup(s, mutex);
    p->run();
}


void loop()
{
}