#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include "Arduino.h"
#include <esp_http_server.h>
#include "cJSON.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "freertos/task.h"
#include "../State/State.h"
#include "../RTC/RTC.h"

class HTTP_Server
{
private:
public:
    static SemaphoreHandle_t sharedMut;
    static State *st;
    static RTC *_rtc;
    static SemaphoreHandle_t _rtcMut;
    HTTP_Server();
    ~HTTP_Server();

    void setup(State *st, SemaphoreHandle_t mut);
    void setRTC(RTC *rtc, SemaphoreHandle_t mut);

    static esp_err_t get_root(httpd_req_t *req);
    static const httpd_uri_t getRoot;

    static esp_err_t get_script(httpd_req_t *req);
    static const httpd_uri_t getScript;

    static esp_err_t get_style(httpd_req_t *req);
    static const httpd_uri_t getStyle;

    static esp_err_t get_favicon_ico(httpd_req_t *req);
    static const httpd_uri_t getFaviconIco;

    static esp_err_t get_status(httpd_req_t *req);
    static const httpd_uri_t getStatus;

    static esp_err_t set_values(httpd_req_t *req);
    static const httpd_uri_t setValues;

    static esp_err_t set_brightness(httpd_req_t *req);
    static const httpd_uri_t setBrightness;

    static esp_err_t start_clock(httpd_req_t *req);
    static const httpd_uri_t startClock;

    static esp_err_t stop_clock(httpd_req_t *req);
    static const httpd_uri_t stopClock;

    static esp_err_t reset_clock(httpd_req_t *req);
    static const httpd_uri_t resetClock;

    static esp_err_t sync_RTC(httpd_req_t *req);
    static const httpd_uri_t syncRTC;

    static esp_err_t program_mode(httpd_req_t *req);
    static const httpd_uri_t programMode;

    static esp_err_t clocks_mode(httpd_req_t *req);
    static const httpd_uri_t clocksMode;

    static esp_err_t buzzer_on(httpd_req_t *req);
    static const httpd_uri_t buzzerOn;
};

#endif