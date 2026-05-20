# alimentação

3 baterias 18650 de 2.5Ah, principalmente por conta da corrente de pico, descarga.

motores dc, wifi e camêra tem alto consumo

esp32s3 ~= 500mA a 800mA
motor dc ~= 1A a 1.5A (e x2 e isso é a corrente de arranque) (difícil dimensionar pq não sei da carga mecânica do robô, o peso e esforço do eixo, n sei como saber exatamente)

~= 3.8A

colocando 3 baterias em paralelo a corrente é dividida, e cada bateria fornece 1/3 do pico, não tendo perigo da tensão de 1 bateria cair em um pico de algum motor e a esp reiniciar, e o capacitor é p segurar esses picos, mas não funcionaria em picos compridos


obs : bom ter um Módulo Proteção para Baterias LI-ION BMS 1S 
https://www.smartkits.com.br/modulo-protecao-para-baterias-li-ion-bms-1s?parceiro=9390&gad_source=1&gad_campaignid=20590456490&gbraid=0AAAAAoi72u3Wh9ChdOx2czgnHXXNvGC0L&gclid=CjwKCAjwt7XQBhBkEiwAtStpp0euI73uIc0B-18Bl3KbLZXY0Cs1ZJsxXlk8wgwyVrN86HPzX9DIzRoCWRwQAvD_BwE
corta a energia se a tensão cai e protege contra curto circuito
