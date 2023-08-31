#include "HTTP_Server.h"

SemaphoreHandle_t HTTP_Server::sharedMut;
State *HTTP_Server::st;
RTC *HTTP_Server::_rtc;
SemaphoreHandle_t HTTP_Server::_rtcMut;
/**
 * @brief Construct a new http server::http server object
 *
 */
HTTP_Server::HTTP_Server()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 13;

    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        Serial.println("Registring uris");
        httpd_register_uri_handler(server, &getRoot);
        httpd_register_uri_handler(server, &getScript);
        httpd_register_uri_handler(server, &getStyle);
        httpd_register_uri_handler(server, &getFaviconIco);
        httpd_register_uri_handler(server, &setValues);
        httpd_register_uri_handler(server, &startClock);
        httpd_register_uri_handler(server, &stopClock);
        httpd_register_uri_handler(server, &getStatus);
        httpd_register_uri_handler(server, &setBrightness);
        httpd_register_uri_handler(server, &resetClock);
        httpd_register_uri_handler(server, &syncRTC);
        httpd_register_uri_handler(server, &programMode);
        httpd_register_uri_handler(server, &clocksMode);
    }
}

/**
 * @brief setup method for initialization of shared data
 *
 * @param st pointer to State
 * @param mut pointer to mutex
 */
void HTTP_Server::setup(State *st, SemaphoreHandle_t mut)
{
    this->st = st;
    this->sharedMut = mut;
}

/**
 * @brief Destroy the http server::http server object
 *
 */
HTTP_Server::~HTTP_Server()
{
}

/**
 * @brief handler for script.js
 *
 * @param req
 * @return esp_err_t
 */
esp_err_t HTTP_Server::get_script(httpd_req_t *req)
{
    extern const unsigned char script_html_start[] asm("_binary_src_WEB_script_js_start");
    extern const unsigned char script_html_end[] asm("_binary_src_WEB_script_js_end");
    const size_t script_html_size = (script_html_end - script_html_start);
    httpd_resp_send(req, (const char *)script_html_start, script_html_size);
    return ESP_OK;
}
/**
 * @brief struct for getScript handler
 * 
 */
const httpd_uri_t HTTP_Server::getScript = {
    .uri = "/script.js",
    .method = HTTP_GET,
    .handler = HTTP_Server::get_script,
    .user_ctx = NULL,
};

/**
 * @brief  handler for style.css
 * 
 * @param req 
 * @return esp_err_t 
 */
esp_err_t HTTP_Server::get_style(httpd_req_t *req)
{
    extern const unsigned char style_html_start[] asm("_binary_src_WEB_style_css_start");
    extern const unsigned char style_html_end[] asm("_binary_src_WEB_style_css_end");
    const size_t style_html_size = (style_html_end - style_html_start);
    httpd_resp_set_type(req, "text/css");
    httpd_resp_send(req, (const char *)style_html_start, style_html_size);
    return ESP_OK;
}

/**
 * @brief struct for getStyle handler
 * 
 */
const httpd_uri_t HTTP_Server::getStyle = {
    .uri = "/style.css",
    .method = HTTP_GET,
    .handler = HTTP_Server::get_style,
    .user_ctx = NULL,
};

/**
 * @brief handler for return index.html
 * 
 * @param req 
 * @return esp_err_t 
 */
esp_err_t HTTP_Server::get_root(httpd_req_t *req)
{
    extern const unsigned char settings_html_start[] asm("_binary_src_WEB_index_html_start");
    extern const unsigned char settings_html_end[] asm("_binary_src_WEB_index_html_end");
    const size_t settings_html_size = (settings_html_end - settings_html_start);
    httpd_resp_send(req, (const char *)settings_html_start, settings_html_size);
    return ESP_OK;
}

/**
 * @brief struct for getRoot handler
 * 
 */
const httpd_uri_t HTTP_Server::getRoot = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = HTTP_Server::get_root,
    .user_ctx = NULL,
};

/**
 * @brief handler for return favicon.ico
 * 
 * @param req 
 * @return esp_err_t 
 */
esp_err_t HTTP_Server::get_favicon_ico(httpd_req_t *req)
{
    extern const unsigned char favicon_ico_start[] asm("_binary_src_WEB_favicon_ico_start");
    extern const unsigned char favicon_ico_end[] asm("_binary_src_WEB_favicon_ico_end");
    const size_t favicon_ico_size = (favicon_ico_end - favicon_ico_start);
    httpd_resp_set_type(req, "image/x-icon");
    httpd_resp_send(req, (const char *)favicon_ico_start, favicon_ico_size);
    return ESP_OK;
}

/**
 * @brief struct for handler getFaviconIco
 * 
 */
const httpd_uri_t HTTP_Server::getFaviconIco = {
    .uri = "/favicon.ico",
    .method = HTTP_GET,
    .handler = HTTP_Server::get_favicon_ico,
    .user_ctx = NULL,
};

/**
 * @brief handler for setting values as university, time
 * 
 * @param req 
 * @return esp_err_t 
 */
esp_err_t HTTP_Server::set_values(httpd_req_t *req)
{
    char *content = (char *)malloc(req->content_len * sizeof(char));
    memset(content, 0, req->content_len);
    int ret = httpd_req_recv(req, content, req->content_len);
    printf("Content: %s\n\r", content);
    cJSON *retjson = cJSON_CreateObject();
    if (ret <= 0)
    {
        cJSON_AddStringToObject(retjson, "status", "400");
    }
    else
    {
        cJSON *values = cJSON_Parse(content);
        if (xSemaphoreTake(sharedMut, 100) == pdPASS)
        {
            string str = cJSON_GetObjectItem(values, "university")->valuestring;
            int time = atoi(cJSON_GetObjectItem(values, "time")->valuestring);
            printf("%s, %d\n\r", str.c_str(), time);
            st->setData(str, time);
            xSemaphoreGive(sharedMut);
        }
        cJSON_AddStringToObject(retjson, "status", "200");
    }
    httpd_resp_send(req, cJSON_PrintUnformatted(retjson), strlen(cJSON_PrintUnformatted(retjson)));
    free(content);
    return ESP_OK;
}

/**
 * @brief struct for handler setValues
 * 
 */
const httpd_uri_t HTTP_Server::setValues = {
    .uri = "/setValues",
    .method = HTTP_POST,
    .handler = HTTP_Server::set_values,
    .user_ctx = NULL,
};

/**
 * @brief handler for set brightness
 * 
 * @param req 
 * @return esp_err_t 
 */
esp_err_t HTTP_Server::set_brightness(httpd_req_t *req)
{
    char *content = (char *)malloc(req->content_len * sizeof(char));
    memset(content, 0, req->content_len);
    int ret = httpd_req_recv(req, content, req->content_len);
    printf("Content: %s\n\r", content);
    cJSON *retjson = cJSON_CreateObject();
    if (ret <= 0)
    {
        cJSON_AddStringToObject(retjson, "status", "400");
    }
    else
    {
        cJSON *values = cJSON_Parse(content);
        if (xSemaphoreTake(sharedMut, 100) == pdPASS)
        {
            int brightness = atoi(cJSON_GetObjectItem(values, "brightness")->valuestring);
            printf(" %d\n\r", brightness);
            st->setBrightness(brightness);
            xSemaphoreGive(sharedMut);
        }
        cJSON_AddStringToObject(retjson, "status", "200");
    }
    httpd_resp_send(req, cJSON_PrintUnformatted(retjson), strlen(cJSON_PrintUnformatted(retjson)));
    free(content);
    return ESP_OK;
}

/**
 * @brief handler for setBrightness
 * 
 */
const httpd_uri_t HTTP_Server::setBrightness = {
    .uri = "/setBrightness",
    .method = HTTP_POST,
    .handler = HTTP_Server::set_brightness,
    .user_ctx = NULL,
};

/**
 * @brief handler for get online/offline status
 * 
 * @param req 
 * @return esp_err_t 
 */
esp_err_t HTTP_Server::get_status(httpd_req_t *req)
{
    cJSON *retjson = cJSON_CreateObject();
    cJSON_AddStringToObject(retjson, "status", "200");
    httpd_resp_send(req, cJSON_PrintUnformatted(retjson), strlen(cJSON_PrintUnformatted(retjson)));
    return ESP_OK;
}

/**
 * @brief struct for handler getStatus
 * 
 */
const httpd_uri_t HTTP_Server::getStatus = {
    .uri = "/getStatus",
    .method = HTTP_GET,
    .handler = HTTP_Server::get_status,
    .user_ctx = NULL,
};

/**
 * @brief handler for starting clocks
 * 
 * @param req 
 * @return esp_err_t 
 */
esp_err_t HTTP_Server::start_clock(httpd_req_t *req)
{
    if (xSemaphoreTake(sharedMut, 100) == pdPASS)
    {
        st->setState(START);
        xSemaphoreGive(sharedMut);
    }
    cJSON *retjson = cJSON_CreateObject();
    cJSON_AddStringToObject(retjson, "status", "200");
    httpd_resp_send(req, cJSON_PrintUnformatted(retjson), strlen(cJSON_PrintUnformatted(retjson)));
    return ESP_OK;
}

/**
 * @brief struct for handler startClock
 * 
 */
const httpd_uri_t HTTP_Server::startClock = {
    .uri = "/startClocks",
    .method = HTTP_POST,
    .handler = HTTP_Server::start_clock,
    .user_ctx = NULL,
};

/**
 * @brief handler for stopping clocks
 * 
 * @param req 
 * @return esp_err_t 
 */
esp_err_t HTTP_Server::stop_clock(httpd_req_t *req)
{
    if (xSemaphoreTake(sharedMut, 100) == pdPASS)
    {
        st->setState(STOP);
        xSemaphoreGive(sharedMut);
    }
    cJSON *retjson = cJSON_CreateObject();
    cJSON_AddStringToObject(retjson, "status", "200");
    httpd_resp_send(req, cJSON_PrintUnformatted(retjson), strlen(cJSON_PrintUnformatted(retjson)));
    return ESP_OK;
}

/**
 * @brief struct for handler stopClock
 * 
 */
const httpd_uri_t HTTP_Server::stopClock = {
    .uri = "/stopClocks",
    .method = HTTP_POST,
    .handler = HTTP_Server::stop_clock,
    .user_ctx = NULL,
};

/**
 * @brief handler for reseting clocks
 * 
 * @param req 
 * @return esp_err_t 
 */
esp_err_t HTTP_Server::reset_clock(httpd_req_t *req)
{
    if (xSemaphoreTake(sharedMut, 100) == pdPASS)
    {
        st->setState(RESET);
        xSemaphoreGive(sharedMut);
    }
    cJSON *retjson = cJSON_CreateObject();
    cJSON_AddStringToObject(retjson, "status", "200");
    httpd_resp_send(req, cJSON_PrintUnformatted(retjson), strlen(cJSON_PrintUnformatted(retjson)));
    return ESP_OK;
}

/**
 * @brief struct for handler resetClock
 * 
 */
const httpd_uri_t HTTP_Server::resetClock = {
    .uri = "/resetClocks",
    .method = HTTP_POST,
    .handler = HTTP_Server::reset_clock,
    .user_ctx = NULL,
};

/**
 * @brief handler for synchronization of RTC
 * 
 * @param req 
 * @return esp_err_t 
 */
esp_err_t HTTP_Server::sync_RTC(httpd_req_t *req)
{
    char *content = (char *)malloc(req->content_len * sizeof(char));
    memset(content, 0, req->content_len);
    int ret = httpd_req_recv(req, content, req->content_len);
    printf("Content: %s\n\r", content);
    cJSON *retjson = cJSON_CreateObject();
    if (ret <= 0)
    {
        cJSON_AddStringToObject(retjson, "status", "400");
    }
    else
    {
        cJSON *values = cJSON_Parse(content);
        if (xSemaphoreTake(_rtcMut, 100) == pdPASS)
        {
            RTC_TIME time;
            time.day = (uint8_t)(cJSON_GetObjectItem(values, "day")->valueint);
            time.hour = (uint8_t)(cJSON_GetObjectItem(values, "hours")->valueint);
            time.minutes = (uint8_t)(cJSON_GetObjectItem(values, "minutes")->valueint);
            time.seconds = (uint8_t)(cJSON_GetObjectItem(values, "seconds")->valueint);
            _rtc->sync(time);
            xSemaphoreGive(_rtcMut);
        }
        cJSON_AddStringToObject(retjson, "status", "200");
    }
    httpd_resp_send(req, cJSON_PrintUnformatted(retjson), strlen(cJSON_PrintUnformatted(retjson)));
    free(content);
    return ESP_OK;
}

/**
 * @brief struct for handler syncRTC
 * 
 */
const httpd_uri_t HTTP_Server::syncRTC = {
    .uri = "/syncRTC",
    .method = HTTP_POST,
    .handler = HTTP_Server::sync_RTC,
    .user_ctx = NULL,
};

void HTTP_Server::setRTC(RTC *rtc, SemaphoreHandle_t mut)
{
    _rtc = rtc;
    _rtcMut = mut;
}

/**
 * @brief handler for changing mode to program mode
 * 
 * @param req 
 * @return esp_err_t 
 */
esp_err_t HTTP_Server::program_mode(httpd_req_t *req)
{
    if (xSemaphoreTake(sharedMut, 100) == pdPASS)
    {
        st->setMode(false);
        xSemaphoreGive(sharedMut);
    }
    cJSON *retjson = cJSON_CreateObject();
    cJSON_AddStringToObject(retjson, "status", "200");
    httpd_resp_send(req, cJSON_PrintUnformatted(retjson), strlen(cJSON_PrintUnformatted(retjson)));
    return ESP_OK;
}

/**
 * @brief struct for handler programMode
 * 
 */
const httpd_uri_t HTTP_Server::programMode = {
    .uri = "/programMode",
    .method = HTTP_POST,
    .handler = HTTP_Server::program_mode,
    .user_ctx = NULL,
};

/**
 * @brief handler for changing mode to clock mode
 * 
 * @param req 
 * @return esp_err_t 
 */
esp_err_t HTTP_Server::clocks_mode(httpd_req_t *req)
{
    if (xSemaphoreTake(sharedMut, 100) == pdPASS)
    {
        st->setMode(true);
        xSemaphoreGive(sharedMut);
    }
    cJSON *retjson = cJSON_CreateObject();
    cJSON_AddStringToObject(retjson, "status", "200");
    httpd_resp_send(req, cJSON_PrintUnformatted(retjson), strlen(cJSON_PrintUnformatted(retjson)));
    return ESP_OK;
}

/**
 * @brief struct for clocksMode
 * 
 */
const httpd_uri_t HTTP_Server::clocksMode = {
    .uri = "/clocksMode",
    .method = HTTP_POST,
    .handler = HTTP_Server::clocks_mode,
    .user_ctx = NULL,
};