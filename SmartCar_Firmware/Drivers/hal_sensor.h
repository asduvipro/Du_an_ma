#ifndef HAL_SENSOR_H
#define HAL_SENSOR_H
#include <stdint.h>
void HAL_Sensor_Init(void);
uint8_t HAL_Sensor_Readline(void);
void HAL_Sensor_MockSet(uint8_t fake_bits);
#endif