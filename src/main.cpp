#include <WiFi.h>
#include "HTTP_SERVER/HTTP_Server.h"
#include "Panel/Panel.h"
#include "State/State.h"
#include "Player/Player.h"
#include "RTC/RTC.h"

const char *ssid = "Panel";        // Enter SSID here
const char *password = "12345678"; // Enter Password here
SemaphoreHandle_t rtcMut;
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

void RTCTask(void *param)
{
    RTC *rtc = static_cast<RTC *>(param);
    uint32_t lastRTCChange = (uint32_t)esp_timer_get_time() / 1000;
    int increments = 0;
    while (true)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
        if ((((uint32_t)esp_timer_get_time() / 1000) - lastRTCChange) >= 1000)
        {
            lastRTCChange = (uint32_t)esp_timer_get_time() / 1000;
            increments++;
            if (xSemaphoreTake(rtcMut, 10) == pdPASS)
            {
                for (int i = 0; i < increments; i++)
                {
                    rtc->incrementTime();
                }
                printf("%d - %d:%d:%d\n\r", rtc->getActualTime().day, rtc->getActualTime().hour, rtc->getActualTime().minutes, rtc->getActualTime().seconds);
                increments = 0;
                xSemaphoreGive(rtcMut);
            }
        }
    }
}

void PanelTask(void *param)
{
    Panel *p = static_cast<Panel *>(param);
    p->run();
}

void setup()
{
    Player *pl = new Player();
    RTC *rtc = new RTC();
    rtcMut = xSemaphoreCreateMutex();
    // pl->playCustom();
    Serial.begin(115200);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    WiFi.softAP(ssid, password);
    HTTP_Server *server = new HTTP_Server();
    SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
    State *s = new State();
    server->setup(s, mutex);
    server->setRTC(rtc, rtcMut);
    TaskHandle_t timeout;
    xTaskCreatePinnedToCore(playerTask, "MusicPlayer", 8192, pl, 1, &timeout, 0);
    xTaskCreatePinnedToCore(RTCTask, "RTC", 8192, rtc, 1, NULL, 0);
    Panel *p = new Panel();
    p->setup(s, mutex, timeout);
    p->setRTC(rtc, rtcMut);
    xTaskCreatePinnedToCore(PanelTask, "Panel", 8192, p, 2, NULL, 1);
}

void loop()
{
}