## Relatório técnico de arquitetura - Entregável Inicial: Documento de Arquitetura e Viabilidade Técnica

* **1. Mapeamento de Hardware e Componentes:** A equipe deve mapear os recursos da placa e os componentes externos do robô:
    * **Identificação:** Modelos exatos dos motores, do driver de motor (ponte H), do sensor de inclinação, do sensor de distância laser, do display (se utilizado localmente) e da câmera.
    * **Pinout (Tabela de GPIOs):** Tabela detalhada especificando quais pinos do ESP32-S3 controlam a direção e a velocidade dos motores (pinos de saída) e quais pinos recebem os dados dos sensores e da câmera.
    * **Alimentação do Sistema:** Como os motores consomem muita corrente, a equipe deve detalhar como será feita a separação ou filtragem da alimentação das baterias entre a parte lógica (ESP32-S3) e a parte de potência (motores), evitando que o microcontrolador reinicie devido a ruídos elétricos ou quedas de tensão no arranque dos motores.

* **r:**

*** Identificação;
- Esp32s3, placa Kiira
- Câmera, OV5640, já na Kiira
- Inclinação, MPU6050, já na Kiira
- Sensor Infra, Módulo Externo VL53LOX
- OLED SSD1306 (podemos usar p ser um menu principal ou menu de manutenção, mostrando IP do WebServer, ou a bateria o status dela)
- Ponte H, Módulo Externo, TB6612FNG
- Motor DC (Talvez 2, acoplados juntos na roda com um enforca gato, tem que ver isso na modelagem do chassi)

*** Pinout (Tabela de GPIOs);

| Componente / Função | Pinos (ESP32-S3) | Barramento / Tipo de Sinal | Notas Técnicas |
| :--- | :--- | :--- | :--- |
| **Câmera OV5640 (Dados)** | GPIOs 16, 17, 18, 12, 10, 8, 9, 11 | Paralelo (Y9-Y2) | Barramento de dados da imagem. |
| **Câmera OV5640 (Controle)** | GPIO 15 (XCLK), 6 (VSYNC), 7 (HREF), 13 (PCLK) | Controle de Sincronismo | Clock e sincronização de quadros. |
| **Câmera OV5640 (Config)** | GPIO 4 (SDA), 5 (SCL) | I2C Dedicado (SIOD/SIOC) | Configuração de registradores. |
| **MPU6050 (Inclinação)** | GPIO 14 (SDA), GPIO 21 (SCL) | I2C0 (400 kHz) | Acelerômetro integrado à placa. |
| **OLED SSD1306 (Display)** | GPIO 14 (SDA), GPIO 21 (SCL) | I2C1 (400 kHz) | Compartilha barramento físico na placa. |
| **VL53L0X (Laser ToF)** | GPIO 14 (SDA), GPIO 21 (SCL) | I2C (Externo) | Ligado em paralelo com o MPU/OLED (endereço único). |
| **Motor Esquerdo (IN1 / IN2)**| GPIO 47, GPIO 48 | Saída Digital / PWM | Utilizando pinos genéricos da placa KIIRA. |
| **Motor Direito (IN1 / IN2)** | GPIO 39, GPIO 40 | Saída Digital / PWM | Utilizando pinos genéricos do ESP32-S3. |

*** Alimentação do Sistema; 

```text

                      +---> [ Kiira: Regulador 3.3V ] ---> ESP32-S3 / Câmera / Sensores
                      |
[ Bateria 18650 ] ----+--  (Nó de Bifurcação VCC)
 (3.7V - 4.2V)        |
                      +---> [ Capacitor Eletrolítico ]
                      |
                      +---> [ Ponte H: Pino VMOT ] ------> Motores DC
                      
* O GND de todos os módulos retorna para o negativo da bateria.

```

* **2. Diagrama de Blocos do Sistema:** Apresentar um esquema gráfico modular do hardware. No centro deve figurar o microcontrolador ESP32-S3 e, ao redor, blocos conectados por setas indicando o fluxo de sinal:
    * **Setas de entrada (In):** Sensor de distância, Sensor de inclinação e Câmera apontando para o ESP32-S3.
    * **Setas de saída (Out):** ESP32-S3 apontando para o Driver de Motores (Ponte H), que por sua vez se conecta às Rodas.
    * **Fluxo de rede:** Seta bidirecional representando a conexão Wi-Fi trocando pacotes WebSockets/HTTP com o dispositivo móvel ou PC do operador.

* **r;**

*** Setas In e Setas OUT

```text

[ Sensor de Distância (Laser ToF) ]
                              |
                              | Sinal (In)
                              v
 [ Câmera ]              +----------+              [ Sensor de Inclinação ]
 [ Óptica ] ---(In)----> | ESP32-S3 | <---(In)---- [ (Giroscópio/Acel.)   ]
                         +----------+
                           |      |
                 Sinal PWM |      | Link Bidirecional (Wi-Fi)
                     (Out) |      | (WebSockets / protocolo)
                           v      v
              [ Driver de Motores ]    [ Interface do Operador ]
              [ (Ponte H / Rodas) ]    [ (Página Web / Mobile) ]

```

```text

                               [ Interface do Operador ]
                                 [  (Web / Smartphone)   ]
                                            ^
                                            | Link Bidirecional (Wi-Fi)
                                            | (protocolos)
                                            v
                                      +----------+
 [ Sensor de Distância ] ----(In)---> |          |
 [      (Laser    )    ]              |          |
                                      |          |
 [ Sensor de Inclinação] ----(In)---> | ESP32-S3 |
 [ (Giroscópio/Acel.)  ]              | (Centro) |
                                      |          |         Sinais de Controle (PWM/Digital)
 [       Câmera        ] ----(In)---> |          | ---(Out)---> [ Driver de Motores ]
 [      (Óptica)       ]              |          |              [     (Ponte H)     ]
                                      +----------+                     |
                                                                       |
                                                                       | Energia / Potência
                                                                       v
                                                                [ Tração Mecânica ]
                                                                [ (Motores / Rodas) ]

```

*** Fluxo de rede;
```text

+------------------------------------------------------+
|               DISPOSITIVO DO OPERADOR                |
|                  Smartphone / PC                     |
+------------------------------------------------------+
                         ^
                         |
                Wi-Fi / HTTP / MJPEG
                         |
                         v


   +--------------+   +--------------+   +--------------+
   |   VL53L0X    |   |    OV5640    |   |   MPU6050    |
   | Laser ToF    |   |  Streaming   |   | Giroscopio   |
   | Anticolisao  |   |    Camera    |   | Acelerometro |
   +------+-------+   +------+-------+   +------+-------+
          |                  |                  |
          | I2C              | Video            | I2C
          v                  v                  v


+------------------------------------------------------+
|                      ESP32-S3                        |
|                    Placa KIIRA                       |
+-------------------------+----------------------------+
                          |
                          | PWM / Controle
                          v


                 +----------------------+
                 |      TB6612FNG       |
                 |    Driver Ponte H    |
                 +----------+-----------+
                            |
                            | Potencia
                            v
                 +----------------------+
                 |      Motores DC      |
                 +----------------------+



+----------------------+
|    Bateria 18650     |
|      3.7V Li-ion     |
+----------+-----------+
           |
           +-----------------------> Driver
           |
           v

+----------------------+
|    Regulador 3.3V    |
|   Alimentacao ESP32  |
+----------------------+

```

* **3. Especificação da Interface Web e Protocolos IoT:**
    * **Arquitetura da Página Web:** Esboço simples da interface do usuário (Wireframe/Layout da Dashboard Web), mostrando onde ficará a tela de vídeo da câmera, os botões de controle de direção e os indicadores visuais do sensor de distância e alertas de inclinação.
    * **Estratégia de Controle Remoto:** Justificativa da escolha do método de comunicação (ex: por que o uso de WebSockets reduzirá o atraso entre o comando do usuário e a resposta física do robô, comparado ao HTTP tradicional).
    * **Modelagem do Payload de Comando:** Definição do formato de string ou caractere enviado pela página ao pressionar cada botão de direção (ex: Comando 'F' para frente, 'B' para trás, 'L' para esquerda, 'R' para direita, 'S' para parar).

* **r;**

*** Arquitetura da Página Web;

*** Estratégia de Controle Remoto;

*** Modelagem do Payload de Comando;

