#ifndef HAL_MOTOR_H
#define HAL_MOTOR_H
#include <stdint.h>
void HAL_Motor_Init(void);
void HAL_Motor_Setspeed(int16_t left_speed, int16_t right_speed);
void HAL_Motor_Coast(void);
void HAL_Motor_Brake(void);
#endif