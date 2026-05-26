#include "webserver.h"

#include "esp_http_server.h"
#include "esp_camera.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include <stdio.h>
#include <string.h>

extern const unsigned char index_html_start[] asm("_binary_index_html_start");
extern const unsigned char index_html_end[]   asm("_binary_index_html_end");

extern const unsigned char style_css_start[] asm("_binary_style_css_start");
extern const unsigned char style_css_end[]   asm("_binary_style_css_end");

extern const unsigned char app_js_start[] asm("_binary_app_js_start");
extern const unsigned char app_js_end[]   asm("_binary_app_js_end");


static const char *TAG = "WEBSERVER";


//http index handler
static esp_err_t index_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");

    httpd_resp_send(
        req,
        (const char *)index_html_start,
        index_html_end - index_html_start
    );

    return ESP_OK;
}
//http style handler
static esp_err_t css_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/css");

    httpd_resp_send(
        req,
        (const char *)style_css_start,
        style_css_end - style_css_start
    );

    return ESP_OK;
}

//http js handler
static esp_err_t js_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/javascript");

    httpd_resp_send(
        req,
        (const char *)app_js_start,
        app_js_end - app_js_start
    );

    return ESP_OK;
}

//streaming handler

static esp_err_t stream_handler(httpd_req_t *req)
{
    camera_fb_t *fb = NULL;
    char header[128];

    httpd_resp_set_type(
        req,
        "multipart/x-mixed-replace; boundary=frame"
    );

    while (true)
    {
        fb = esp_camera_fb_get();

        if (!fb)
        {
            ESP_LOGE(TAG, "Falha captura frame");
            continue;
        }

        int len = snprintf(
            header,
            sizeof(header),
            "--frame\r\n"
            "Content-Type: image/jpeg\r\n"
            "Content-Length: %u\r\n\r\n",
            fb->len
        );

        if (httpd_resp_send_chunk(req, header, len) != ESP_OK)
        {
            esp_camera_fb_return(fb);
            break;
        }

        if (httpd_resp_send_chunk(
                req,
                (const char *)fb->buf,
                fb->len) != ESP_OK)
        {
            esp_camera_fb_return(fb);
            break;
        }

        if (httpd_resp_send_chunk(req, "\r\n", 2) != ESP_OK)
        {
            esp_camera_fb_return(fb);
            break;
        }

        esp_camera_fb_return(fb);

        vTaskDelay(pdMS_TO_TICKS(50));
    }

    return ESP_OK;
}

//webserver
static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_uri_t index_uri = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = index_handler,
            .user_ctx = NULL
        };

        httpd_uri_t stream_uri = {
            .uri = "/stream",
            .method = HTTP_GET,
            .handler = stream_handler,
            .user_ctx = NULL
        };
        
        httpd_uri_t css_uri = {
            .uri = "/style.css",
            .method = HTTP_GET,
            .handler = css_handler,
            .user_ctx = NULL
        };

        httpd_uri_t js_uri = {
            .uri = "/app.js",
            .method = HTTP_GET,
            .handler = js_handler,
            .user_ctx = NULL
        };

        httpd_register_uri_handler(server, &index_uri);
        httpd_register_uri_handler(server, &stream_uri);
        httpd_register_uri_handler(server,&css_uri);
        httpd_register_uri_handler(server,&js_uri);
    }

    return server;
}

void webserver_start(void)
{
    start_webserver();
    ESP_LOGI(TAG, "Servidor HTTP iniciado");
}
