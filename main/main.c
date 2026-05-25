#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_camera.h"
#include "esp_log.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"

#include "esp_netif.h"
#include "esp_http_server.h"


/* ==========================
   WIFI
   ========================== */

#define WIFI_SSID      "Galaxy S25 3499"
#define WIFI_PASSWORD  "9tc6cgkpqtjg8eq"

/* ==========================
   CAMERA OV5640
   ========================== */

#define CAM_XCLK   15

#define CAM_SDA    4
#define CAM_SCL    5

#define CAM_Y9     16
#define CAM_Y8     17
#define CAM_Y7     18
#define CAM_Y6     12
#define CAM_Y5     10
#define CAM_Y4     8
#define CAM_Y3     9
#define CAM_Y2     11

#define CAM_VSYNC  6
#define CAM_HREF   7
#define CAM_PCLK   13

static const char *TAG = "CAM_STREAM";

/* ==========================
   PAGINA HTML
   ========================== */

static esp_err_t index_handler(httpd_req_t *req)
{
    const char *html =
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<meta charset='utf-8'>"
        "<title>OV5640 Stream</title>"
        "</head>"
        "<body>"
        "<h2>ESP32-S3 + OV5640</h2>"
        "<img src='/stream'>"
        "</body>"
        "</html>";

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

/* ==========================
   STREAM MJPEG
   ========================== */

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



/* ==========================
   WIFI
   ========================== */

static void wifi_init_sta(void)
{
    esp_netif_init();

    esp_event_loop_create_default();

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
        },
    };

    esp_wifi_set_mode(WIFI_MODE_STA);

    esp_wifi_set_config(
        WIFI_IF_STA,
        &wifi_config
    );

    esp_wifi_start();

    esp_wifi_connect();
    esp_netif_ip_info_t ip_info;

vTaskDelay(pdMS_TO_TICKS(5000));

esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

if (netif) {
    esp_netif_get_ip_info(netif, &ip_info);

    ESP_LOGI(TAG,
             "IP: " IPSTR,
             IP2STR(&ip_info.ip));
}

    ESP_LOGI(TAG, "Conectando WiFi...");

    
}

/* ==========================
   WEBSERVER
   ========================== */

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

        httpd_register_uri_handler(server, &index_uri);
        httpd_register_uri_handler(server, &stream_uri);
    }

    return server;
}


/* ==========================
   APP MAIN
   ========================== */

void app_main(void)
{
    ESP_ERROR_CHECK(
        nvs_flash_init()
    );

    wifi_init_sta();

    camera_config_t config = {
        .pin_pwdn = -1,
        .pin_reset = -1,

        .pin_xclk = CAM_XCLK,

        .pin_sccb_sda = CAM_SDA,
        .pin_sccb_scl = CAM_SCL,

        .pin_d7 = CAM_Y9,
        .pin_d6 = CAM_Y8,
        .pin_d5 = CAM_Y7,
        .pin_d4 = CAM_Y6,
        .pin_d3 = CAM_Y5,
        .pin_d2 = CAM_Y4,
        .pin_d1 = CAM_Y3,
        .pin_d0 = CAM_Y2,

        .pin_vsync = CAM_VSYNC,
        .pin_href  = CAM_HREF,
        .pin_pclk  = CAM_PCLK,

        .xclk_freq_hz = 10000000,

        .pixel_format = PIXFORMAT_JPEG,

        .frame_size = FRAMESIZE_QVGA,

        .jpeg_quality = 15,

        .fb_count = 1,

        .grab_mode = CAMERA_GRAB_WHEN_EMPTY,

        .fb_location = CAMERA_FB_IN_DRAM
    };

    ESP_LOGI(TAG, "Inicializando camera...");

    esp_err_t err = esp_camera_init(&config);

    if (err != ESP_OK)
    {
        
        ESP_LOGE(
            TAG,
            "Falha camera: 0x%x",
            err
        );
        return;
    }

    ESP_LOGI(TAG, "Camera OK");

    start_webserver();

    ESP_LOGI(
        TAG,
        "Servidor HTTP iniciado"
    );

    while (1)
    {
        vTaskDelay(
            pdMS_TO_TICKS(1000)
        );
    }
}