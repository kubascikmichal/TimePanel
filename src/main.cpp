#include <WiFi.h>
#include "HTTP_SERVER/HTTP_Server.h"
#include "Panel/Panel.h"
#include "State/State.h"
#include "Player/Player.h"

const char *ssid = "Panel";        // Enter SSID here
const char *password = "12345678"; // Enter Password here

void playerTask(void *param)
{
    Player *player = static_cast<Player *>(param);
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100);
    while (true)
    {
        uint32_t ret = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
        if (ret > 0)
        {
            player->playCustom();
        }
    }
}

void setup()
{
    Player *pl = new Player();
    pl->playCustom();
    Serial.begin(115200);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    WiFi.softAP(ssid, password);
    HTTP_Server *server = new HTTP_Server();
    SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
    State *s = new State();
    server->setup(s, mutex);
    TaskHandle_t timeout;
    xTaskCreatePinnedToCore(playerTask, "MusicPlayer", 8192, pl, 1, &timeout, 0);
    Panel *p = new Panel();
    p->setup(s, mutex, timeout);
    p->run();
}

void loop()
{
}