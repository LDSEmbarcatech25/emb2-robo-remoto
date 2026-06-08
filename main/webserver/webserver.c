#include "webserver.h"

#include "esp_http_server.h"
#include "esp_camera.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include <stdio.h>
#include <string.h>

#include "motor/motor.h"
#include "pwm/pwm.h"
#include "bateria/bateria.h"
#include "wifi/wifi.h"

extern const unsigned char index_html_start[] asm("_binary_index_html_start");
extern const unsigned char index_html_end[]   asm("_binary_index_html_end");

extern const unsigned char style_css_start[] asm("_binary_style_css_start");
extern const unsigned char style_css_end[]   asm("_binary_style_css_end");

extern const unsigned char app_js_start[] asm("_binary_app_js_start");
extern const unsigned char app_js_end[]   asm("_binary_app_js_end");

extern const unsigned char imagemcamera_jpg_start[] asm("_binary_imagemcamera_jpg_start");
extern const unsigned char imagemcamera_jpg_end[] asm("_binary_imagemcamera_jpg_end");

static const char *TAG = "WEBSERVER";

//handler websocket voltando
static esp_err_t ws_handler(httpd_req_t *req)
{
    // handshake
    if (req->method == HTTP_GET)
    {
        ESP_LOGI(TAG, "WebSocket conectado");
        return ESP_OK;
    }

    httpd_ws_frame_t ws_pkt;

    memset(&ws_pkt, 0, sizeof(ws_pkt));

    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    esp_err_t ret =
        httpd_ws_recv_frame(req, &ws_pkt, 0);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Erro recebendo frame");
        return ret;
    }

    if (ws_pkt.len > 0)
    {
        uint8_t *buf = calloc(1, ws_pkt.len + 1);

        ws_pkt.payload = buf;

        ret = httpd_ws_recv_frame(
            req,
            &ws_pkt,
            ws_pkt.len
        );

        if (ret == ESP_OK)
        {
            ESP_LOGI(
                TAG,
                "COMANDO: %s",
                (char*)ws_pkt.payload
            );
        }
  
        char *cmd = (char*)ws_pkt.payload;

        if(strcmp(cmd, "FRENTE") == 0)
        {
            motor_frente();
        }
        else if(strcmp(cmd, "TRAS") == 0)
        {
            motor_tras();
        }
        else if(strcmp(cmd, "ESQUERDA") == 0)
        {
            motor_esquerda();
        }
        else if(strcmp(cmd, "DIREITA") == 0)
        {
            motor_direita();
        }
        else if(strcmp(cmd, "PARAR") == 0)
        {
            motor_parar();
        }

        int bateria = bateria_percentual();

        char resposta[128];
        sprintf(resposta, "Velocidade:%d cm/s \n Bateria:%d%% \n IP:%s", velocidade_cm, bateria, ip_addr);

        httpd_ws_frame_t ws_res ={
            .final = true,
            .fragmented = false,
            .type = HTTPD_WS_TYPE_TEXT,
            .payload = (uint8_t*)resposta,
            .len = strlen(resposta)
        };

        httpd_ws_send_frame(req, &ws_res);

        free(buf);
    }

    return ESP_OK;
}

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
    camera_fb_t *fb = esp_camera_fb_get();

    if (!fb)
    {
        ESP_LOGE(TAG, "Falha captura frame");

        httpd_resp_send_500(req);

        return ESP_FAIL;
    }

    httpd_resp_set_type(req, "image/jpeg");

    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");

    esp_err_t res = httpd_resp_send(
        req,
        (const char *)fb->buf,
        fb->len
    );

    esp_camera_fb_return(fb);

    return res;
}

//handler da imagem de login
static esp_err_t imagemcamera_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "image/jpg");

    httpd_resp_send(
        req,
        (const char *)imagemcamera_jpg_start,
        imagemcamera_jpg_end - imagemcamera_jpg_start
    );

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

        httpd_uri_t ws_uri = {
            .uri = "/ws",
            .method = HTTP_GET,
            .handler = ws_handler,
            .user_ctx = NULL,
            .is_websocket = true
        };

        httpd_uri_t imagemcamera_uri = {
            .uri = "/imagemcamera.jpg",
            .method = HTTP_GET,
            .handler = imagemcamera_handler,
            .user_ctx = NULL
        };
        
        httpd_register_uri_handler(server, &index_uri);
        httpd_register_uri_handler(server, &stream_uri);
        httpd_register_uri_handler(server,&css_uri);
        httpd_register_uri_handler(server,&js_uri);
        httpd_register_uri_handler(server, &ws_uri);
        httpd_register_uri_handler(server, &imagemcamera_uri);
        
    
    }

    return server;
}

void webserver_start(void)
{
    start_webserver();
    ESP_LOGI(TAG, "Servidor HTTP iniciado");
}