#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include "Arduino.h"
#include <esp_http_server.h>
#include "cJSON.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

class HTTP_Server
{
private:
public:
    static SemaphoreHandle_t sharedMut;
    HTTP_Server();
    ~HTTP_Server();

    static esp_err_t get_root(httpd_req_t *req);
    static const httpd_uri_t getRoot;

    static esp_err_t get_script(httpd_req_t *req);
    static const httpd_uri_t getScript;

    static esp_err_t get_style(httpd_req_t *req);
    static const httpd_uri_t getStyle;

    static esp_err_t get_favicon_ico(httpd_req_t *req);
    static const httpd_uri_t getFaviconIco;
};

#endif