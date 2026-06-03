#include "motor.h"
#include "driver/gpio.h"
#include "../pwm/pwm.h"


#define MOTOR1_IN1 41
#define MOTOR1_IN2 42
#define MOTOR1_VEL 43

#define MOTOR2_IN1 14 //SDA
#define MOTOR2_IN2 21 //SCL
#define MOTOR2_VEL 44

int velocidade_pwm = 180;
int velocidade_cm = 0;


void motor_init(void)
{
    gpio_reset_pin(MOTOR1_IN1);
    gpio_reset_pin(MOTOR1_IN2);
    gpio_reset_pin(MOTOR2_IN1);
    gpio_reset_pin(MOTOR2_IN2);

    gpio_set_direction(MOTOR1_IN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR1_IN2, GPIO_MODE_OUTPUT);

    gpio_set_direction(MOTOR2_IN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR2_IN2, GPIO_MODE_OUTPUT);

    pwm_init();
    motor_parar();
}

void motor_frente(void)
{
    velocidade_cm = (velocidade_pwm * 80) / 255;
    gpio_set_level(MOTOR1_IN1, 1);
    gpio_set_level(MOTOR1_IN2, 0);

    gpio_set_level(MOTOR2_IN1, 0);
    gpio_set_level(MOTOR2_IN2, 1); //invertido

    pwm_set_motor1(120); 
    pwm_set_motor2(120); 
}

void motor_tras(void)
{
    velocidade_cm = (velocidade_pwm * 80) / 255;
    gpio_set_level(MOTOR1_IN1, 0);
    gpio_set_level(MOTOR1_IN2, 1);

    gpio_set_level(MOTOR2_IN1, 1);
    gpio_set_level(MOTOR2_IN2, 0); //invertido

    pwm_set_motor1(120);
    pwm_set_motor2(120);
}

void motor_esquerda(void)
{
    velocidade_cm = (velocidade_pwm * 80) / 255;
    gpio_set_level(MOTOR1_IN1, 1);
    gpio_set_level(MOTOR1_IN2, 0);

    gpio_set_level(MOTOR2_IN1, 0);
    gpio_set_level(MOTOR2_IN2, 0);

    pwm_set_motor1(100);
    pwm_set_motor2(100);
}

void motor_direita(void)
{
    velocidade_cm = (velocidade_pwm * 80) / 255;
    gpio_set_level(MOTOR1_IN1, 0);
    gpio_set_level(MOTOR1_IN2, 0);

    gpio_set_level(MOTOR2_IN1, 0);
    gpio_set_level(MOTOR2_IN2, 1); //invertido

    pwm_set_motor1(100);
    pwm_set_motor2(100);
}

void motor_parar(void)
{
    velocidade_cm = 0;
    gpio_set_level(MOTOR1_IN1, 0);
    gpio_set_level(MOTOR1_IN2, 0);

    gpio_set_level(MOTOR2_IN1, 0);
    gpio_set_level(MOTOR2_IN2, 0);

    pwm_set_motor1(0); // Parar o motor 1
    pwm_set_motor2(0); // Parar o motor 2
}

