```text
Estrutura do projeto

robo_remoto/
│
├── include/                  # headers (definições)
│   ├── pinout.h              # GPIOs físicos
│   ├── config.h              # parâmetros do sistema
│   ├── system_state.h        # estado global do robô
│   ├── camera_config.h       # config da câmera
│   ├── wifi_config.h         # rede
│   └── protocol.h            # comandos WebSocket (F,B,L,R,S)
│
├── src/                      # código fonte principal
│   ├── main.c               # entry point
│   ├── system_state.c       # instância global do estado
│   │
│   ├── motors/
│   │   ├── motor_driver.c   # controle ponte H / PWM
│   │   └── motor_driver.h
│   │
│   ├── sensors/
│   │   ├── imu.c            # acelerômetro/giroscópio
│   │   ├── tof.c            # sensor distância (VL53L0X)
│   │   └── sensors.h
│   │
│   ├── camera/
│   │   ├── camera.c         # OV2640 stream MJPEG
│   │   └── camera.h
│   │
│   ├── wifi/
│   │   ├── wifi_manager.c   # conexão Wi-Fi / AP mode
│   │   └── wifi_manager.h
│   │
│   ├── web/
│   │   ├── webserver.c      # HTTP server
│   │   ├── websocket.c      # controle em tempo real
│   │   └── webserver.h
│   │
│   ├── safety/
│   │   ├── safety.c         # anticolisão + capotamento
│   │   └── safety.h
│   │
│   ├── control/
│   │   ├── command_parser.c # interpreta F,B,L,R,S
│   │   └── control.h
│   │
│   └── utils/
│       ├── json_builder.c   # monta telemetria JSON
│       └── utils.h
│
├── web/                      # interface web (SPIFFS/LittleFS)
│   ├── index.html
│   ├── style.css
│   ├── app.js               # controle + WebSocket
│
├── build/                   # compilação
├── partitions.csv           # ESP32 partitions
└── platformio.ini (ou CMakeLists.txt)  
