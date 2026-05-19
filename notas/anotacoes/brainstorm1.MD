# PROJETO DE ENCERRAMENTO DE RESIDÊNCIA
### DISPOSITIVO IoT PARA EXPLORAÇÃO REMOTA E ROBÓTICA MÓVEL EM LOCAIS DE DIFÍCIL ACESSO

# O que eu entendo
É como se fosse um robô/carrinho patrulheiro, que fará inspeções em dutos (dentro de um espaço cilindríco, pequeno e escuro) para averigar entupimento no local de díficil acesso, alem disso, esse carrinho também pede que em uma página web desktop E mobile,(podemos fazer com html/css e hospedar em um canal que já deixe a página responsiva, pesquisar talvez netfily, usei um tempo atrás) e dentro dessa página teremos a transmissão ao vivo do carrinho e o controle de front, back, left e right do carrinho.
É bom, e eu não entendi se obrigatório, talvez seja e por isso falou do sensor a laser, mas incluir condições de limite, para evitar que mesmo que a nuvem caia ou o controle mande o robo bater no duto, ele tenha um sensor ultrassônico com condições que o impeça disso, p segurança própria do produto

# O que usaremos
** Camêra == Kiira, onde já tem a câmera integrada
** Sensor de Inclinação (Giroscópio/Acelerômetro) == Integrado na Kiira já, MPU6050, I2C0 pinos SDA e SCL, 14 e 21
** Sensor de Distância a Laser (VL53L0X) == mesmo barramento do acelerometro, I2C0 (organizar pinagem)
** integrar uma Ponte H externa (como o módulo L298N ou DRV8833) interligada aos pinos livres do ESP32-S3 para aplicar o controle PWM direcional completo (B, F, R, L)

# O que ainda não fiquei mt entendida
** Rodas

# Links:
https://www.reddit.com/r/esp32/comments/1cn9pur/when_to_use_freertos_with_esp32/?tl=pt-br == acho que devemos aplicar p aproveitar melhor gerenciamento dos nucleos do hardware

https://www.makerguides.com/pt/stream-video-with-with-xiao-esp32-s3-sense-pt/ == 