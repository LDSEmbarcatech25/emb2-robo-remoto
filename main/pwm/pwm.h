#ifndef PWM_H
#define PWM_H

#include "stdio.h"

void pwm_init(void);
void pwm_set_motor1(uint32_t vel);
void pwm_set_motor2(uint32_t vel);

#endif