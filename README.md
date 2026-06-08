<<<<<<< HEAD
# PROJETO DE ENCERRAMENTO DE RESIDÊNCIA
### DISPOSITIVO IoT PARA EXPLORAÇÃO REMOTA E ROBÓTICA MÓVEL EM LOCAIS DE DIFÍCIL ACESSO

## 1. O Problema 

Sou Engenheiro de Segurança Operacional em uma empresa de mineração e infraestrutura petroquímica. Frequentemente, precisamos inspecionar o interior de tubulações desativadas, galerias subterrâneas estreitas ou áreas com suspeita de vazamento de gases tóxicos. Enviar um técnico humano para esses locais é extremamente perigoso e, muitas vezes, fisicamente impossível devido ao espaço reduzido. As soluções de mercado com robôs cabeados são muito caras e limitadas pelo comprimento do cabo.

Precisamos de uma solução em rede para exploração remota (veículo robótico móvel - Rover). Esse robô deve ser controlado em tempo real por um operador através de uma página web amigável, acessível tanto por computadores quanto por smartphones. O operador guiará o robô (frente, trás, direita e esquerda) baseando-se visualmente no streaming de vídeo ao vivo gerado pela câmera acoplada ao veículo, que será transmitido diretamente na mesma interface de controle.

O veículo deve operar com duas salvaguardas automáticas de hardware e firmware:

* **Sistema Anticolisão:** O sensor de distância a laser (ToF) deve monitorar constantemente a rota frontal. Se o operador comandar o robô para frente, mas o sensor detectar um obstáculo muito próximo, o firmware deve aplicar um freio automático imediato, ignorando o comando humano para evitar uma colisão que destruiria o equipamento.
* **Monitoramento de Capotamento:** Por operar em terrenos irregulares e escombros, o robô deve monitorar continuamente um sensor de inclinação (giroscópio/acelerômetro). Caso o veículo incline além do limite seguro ou vire de cabeça para baixo, o sistema deve interromper a tração das rodas por segurança e enviar um alerta visual imediato na página web para avisar o operador.

## 2. Recomendações de Firmware e Conectividade

* **Servidor Web Embarcado (Webserver):** O ESP32-S3 deve rodar um servidor HTTP e um serviço de comunicação de baixa latência para lidar com duas tarefas simultâneas: servir a página HTML/CSS/JS de controle e fazer o streaming de vídeo.
* **Controle de Baixa Latência (WebSockets):** Para que os comandos de movimentação (frente, trás, etc.) e as atualizações dos sensores (distância e inclinação) sejam instantâneos, recomenda-se fortemente o uso de WebSockets em vez de requisições HTTP comuns. Isso cria um canal de comunicação bidirecional aberto e contínuo.
* **Streaming de Vídeo:** O fluxo da câmera deve ser transmitido para a página web na forma de um stream MJPEG ou técnica similar de atualização rápida de frames, balanceando a resolução (sugestão: CIF ou QVGA) para garantir uma boa taxa de quadros por segundo (FPS) sem travar a rede Wi-Fi.
* **Lógica de Interrupção Anticolisão:** A leitura do sensor VL53L0X deve rodar em uma tarefa de alta prioridade. Se a distância lida for menor que o limite de segurança (ex: 15 cm), o firmware deve cortar imediatamente o sinal dos motores que geram o movimento para frente, permitindo apenas comandos de rotação ou marcha ré.
* **Controle de Tração dos Motores:** O controle das rodas deve ser feito aplicando sinais digitais ou modulação por largura de pulso (PWM) nos pinos que controlam a ponte H (driver de motores da placa), garantindo suavidade nas curvas e paradas.

## 3. Requisitos de Modelagem 3D (Gabinete e Chassi)

Este projeto envolve o desenvolvimento de um produto dinâmico em movimento, exigindo atenção extra à distribuição de peso e resistência mecânica:

* **Chassi e Integração das Rodas:** Projetar a estrutura mecânica inferior (chassi) que servirá de base para a fixação dos motores DC (ou servo motores modificados) e o encaixe das rodas. O design deve prever o alinhamento correto dos eixos para que o robô ande em linha reta.
* **Gabinete de Proteção Superior:** Um case para proteger a eletrônica sensível (ESP32-S3 e baterias) contra quedas de detritos ou poeira do ambiente de exploração.
* **Alinhamento dos Sensores Ópticos e Dinâmicos:**
    * A câmera deve ser fixada na parte frontal superior com uma angulação que permita enxergar o chão logo à frente e o horizonte da tubulação.
    * O sensor laser de distância deve ser posicionado na parte mais frontal do chassi, perfeitamente paralelo ao solo, para medir obstáculos sem apontar para o próprio chão.
    * O sensor de inclinação deve ser fixado firmemente e centralizado no meio do chassi para registrar os desvios de ângulo reais do veículo, sem sofrer com folgas ou balanços internos.

## 4. Entregável Inicial: Documento de Arquitetura e Viabilidade Técnica

Antes de iniciar o desenvolvimento prático do firmware ou a modelagem 3D do chassi, a equipe deverá entregar um relatório técnico de arquitetura contendo obrigatoriamente:

* **4.1. Mapeamento de Hardware e Componentes:** A equipe deve mapear os recursos da placa e os componentes externos do robô:
    * **Identificação:** Modelos exatos dos motores, do driver de motor (ponte H), do sensor de inclinação, do sensor de distância laser, do display (se utilizado localmente) e da câmera.
    * **Pinout (Tabela de GPIOs):** Tabela detalhada especificando quais pinos do ESP32-S3 controlam a direção e a velocidade dos motores (pinos de saída) e quais pinos recebem os dados dos sensores e da câmera.
    * **Alimentação do Sistema:** Como os motores consomem muita corrente, a equipe deve detalhar como será feita a separação ou filtragem da alimentação das baterias entre a parte lógica (ESP32-S3) e a parte de potência (motores), evitando que o microcontrolador reinicie devido a ruídos elétricos ou quedas de tensão no arranque dos motores.

* **4.2. Diagrama de Blocos do Sistema:** Apresentar um esquema gráfico modular do hardware. No centro deve figurar o microcontrolador ESP32-S3 e, ao redor, blocos conectados por setas indicando o fluxo de sinal:
    * **Setas de entrada (In):** Sensor de distância, Sensor de inclinação e Câmera apontando para o ESP32-S3.
    * **Setas de saída (Out):** ESP32-S3 apontando para o Driver de Motores (Ponte H), que por sua vez se conecta às Rodas.
    * **Fluxo de rede:** Seta bidirecional representando a conexão Wi-Fi trocando pacotes WebSockets/HTTP com o dispositivo móvel ou PC do operador.

* **4.3. Especificação da Interface Web e Protocolos IoT:**
    * **Arquitetura da Página Web:** Esboço simples da interface do usuário (Wireframe/Layout da Dashboard Web), mostrando onde ficará a tela de vídeo da câmera, os botões de controle de direção e os indicadores visuais do sensor de distância e alertas de inclinação.
    * **Estratégia de Controle Remoto:** Justificativa da escolha do método de comunicação (ex: por que o uso de WebSockets reduzirá o atraso entre o comando do usuário e a resposta física do robô, comparado ao HTTP tradicional).
    * **Modelagem do Payload de Comando:** Definição do formato de string ou caractere enviado pela página ao pressionar cada botão de direção (ex: Comando 'F' para frente, 'B' para trás, 'L' para esquerda, 'R' para direita, 'S' para parar).
=======
# IoT Rover

Projeto de um rover controlado por Wi-Fi utilizando um ESP32-S3.

## Funcionalidades

- Controle remoto via interface web
- Comunicação em tempo real com WebSocket
- Controle de dois motores DC
- Operação sem fio por rede Wi-Fi

## Tecnologias

- ESP32-S3
- PlatformIO
- C++
- HTML/CSS/JavaScript
- WebSocket

## Objetivo

Desenvolver um veículo robótico de baixo custo para estudos de Internet das Coisas (IoT), sistemas embarcados e comunicação sem fio.

## Autor

Layane
>>>>>>> projeto-final/main
