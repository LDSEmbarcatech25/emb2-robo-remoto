#include <stdio.h>

#include "esp_camera.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define CAM_XCLK 15

#define CAM_SDA 4
#define CAM_SCL 5

#define CAM_Y9 16
#define CAM_Y8 17
#define CAM_Y7 18
#define CAM_Y6 12
#define CAM_Y5 10
#define CAM_Y4 8
#define CAM_Y3 9
#define CAM_Y2 11

#define CAM_VSYNC 6
#define CAM_HREF   7
#define CAM_PCLK 13

static const char *TAG = "CAM_TEST";

void app_main(void)
{
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

        .xclk_freq_hz = 10000000,     // 10 MHz para teste
        .pixel_format = PIXFORMAT_JPEG,
        .frame_size = FRAMESIZE_QVGA, // 320x240
        .jpeg_quality = 15,
        .fb_count = 1,
        .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
        .fb_location = CAMERA_FB_IN_DRAM,
    };

    ESP_LOGI(TAG, "Inicializando camera...");

    esp_err_t err = esp_camera_init(&config);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha esp_camera_init(): 0x%x", err);
        return;
    }

    ESP_LOGI(TAG, "Camera inicializada!");

    camera_fb_t *fb = esp_camera_fb_get();

    if (!fb) {
        ESP_LOGE(TAG, "Falha ao capturar frame");
        return;
    }

    ESP_LOGI(TAG, "FRAME OK");
    ESP_LOGI(TAG, "Largura : %d", fb->width);
    ESP_LOGI(TAG, "Altura  : %d", fb->height);
    ESP_LOGI(TAG, "Bytes   : %u", fb->len);

    esp_camera_fb_return(fb);

    ESP_LOGI(TAG, "Teste concluido.");

   while (1)
{
    camera_fb_t *fb = esp_camera_fb_get();

    if (!fb) {
        ESP_LOGE(TAG, "Falha ao capturar frame");
        vTaskDelay(pdMS_TO_TICKS(5000));
        continue;
    }

    ESP_LOGI(TAG, "FRAME OK");
    ESP_LOGI(TAG, "Largura : %d", fb->width);
    ESP_LOGI(TAG, "Altura  : %d", fb->height);
    ESP_LOGI(TAG, "Bytes   : %u", fb->len);

    esp_camera_fb_return(fb);

    vTaskDelay(pdMS_TO_TICKS(5000));
}
}