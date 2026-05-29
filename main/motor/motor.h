#ifndef MOTOR_H
#define MOTOR_H

extern int velocidade_cm;


void motor_init(void);

void motor_frente(void);
void motor_tras(void);
void motor_esquerda(void);
void motor_direita(void);

void motor_parar(void);

#endif