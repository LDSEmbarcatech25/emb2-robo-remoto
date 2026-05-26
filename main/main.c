#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "wifi/wifi.h"
#include "camera/camera.h"
#include "webserver/webserver.h"

//app main

void app_main(void)
{
    ESP_ERROR_CHECK(
        nvs_flash_init()
    );

    wifi_init();

    ESP_ERROR_CHECK(
        camera_init()
    );

    webserver_start();

    while (1)
    {
        vTaskDelay(
            pdMS_TO_TICKS(1000)
        );
    }
}