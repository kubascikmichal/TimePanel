#include "HTTP_Server.h"

HTTP_Server::HTTP_Server()
{
    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;

    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK)
    {
        /* Register URI handlers */
        Serial.println("Registring uris");
        httpd_register_uri_handler(server, &getRoot);
        httpd_register_uri_handler(server, &getScript);
        httpd_register_uri_handler(server, &getStyle);
        httpd_register_uri_handler(server, &getFaviconIco);
    }
}

HTTP_Server::~HTTP_Server()
{
}

esp_err_t HTTP_Server::get_script(httpd_req_t *req)
{
    extern const unsigned char script_html_start[] asm("_binary_src_WEB_script_js_start");
    extern const unsigned char script_html_end[] asm("_binary_src_WEB_script_js_end");
    const size_t script_html_size = (script_html_end - script_html_start);
    httpd_resp_send(req, (const char *)script_html_start, script_html_size);
    return ESP_OK;
}
const httpd_uri_t HTTP_Server::getScript = {
    .uri = "/script.js",
    .method = HTTP_GET,
    .handler = HTTP_Server::get_script,
    .user_ctx = NULL,
};
esp_err_t HTTP_Server::get_style(httpd_req_t *req)
{
    extern const unsigned char style_html_start[] asm("_binary_src_WEB_style_css_start");
    extern const unsigned char style_html_end[] asm("_binary_src_WEB_style_css_end");
    const size_t style_html_size = (style_html_end - style_html_start);
    httpd_resp_set_type(req, "text/css");
    httpd_resp_send(req, (const char *)style_html_start, style_html_size);
    return ESP_OK;
}
const httpd_uri_t HTTP_Server::getStyle = {
    .uri = "/style.css",
    .method = HTTP_GET,
    .handler = HTTP_Server::get_style,
    .user_ctx = NULL,
};

esp_err_t HTTP_Server::get_root(httpd_req_t *req)
{
    extern const unsigned char settings_html_start[] asm("_binary_src_WEB_index_html_start");
    extern const unsigned char settings_html_end[] asm("_binary_src_WEB_index_html_end");
    const size_t settings_html_size = (settings_html_end - settings_html_start);
    httpd_resp_send(req, (const char *)settings_html_start, settings_html_size);
    return ESP_OK;
}
const httpd_uri_t HTTP_Server::getRoot = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = HTTP_Server::get_root,
    .user_ctx = NULL,
};

esp_err_t HTTP_Server::get_favicon_ico(httpd_req_t *req)
{
    extern const unsigned char favicon_ico_start[] asm("_binary_src_WEB_favicon_ico_start");
    extern const unsigned char favicon_ico_end[] asm("_binary_src_WEB_favicon_ico_end");
    const size_t favicon_ico_size = (favicon_ico_end - favicon_ico_start);
    httpd_resp_set_type(req, "image/x-icon");
    httpd_resp_send(req, (const char *)favicon_ico_start, favicon_ico_size);
    return ESP_OK;
}
const httpd_uri_t HTTP_Server::getFaviconIco = {
    .uri = "/favicon.ico",
    .method = HTTP_GET,
    .handler = HTTP_Server::get_favicon_ico,
    .user_ctx = NULL,
};