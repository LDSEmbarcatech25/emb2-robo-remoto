idf_component_register(
    SRCS 
        "main.c"
        "wifi/wifi.c"
        "camera/camera.c"
        "webserver/webserver.c"
        "motor/motor.c"
        "pwm/pwm.c"
        "bateria/bateria.c"

    INCLUDE_DIRS
        "."
        "wifi"
        "camera"
        "webserver"
        "motor"
        "pwm"
        "bateria"


    EMBED_FILES
        "ui/index.html"
        "ui/style.css"
        "ui/app.js"

    REQUIRES
        esp_wifi
        esp_http_server
        esp_driver_gpio
        esp_timer
        nvs_flash
        esp_netif
        esp_event
        esp32-camera
        esp_adc
    )