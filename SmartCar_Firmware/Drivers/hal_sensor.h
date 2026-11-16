#ifndef HAL_SENSOR_H
#define HAL_SENSOR_H
#include <stdint.h>
#define SENSOR_RIGHT_MASK   (1U << 0) // A0 (PC0)
#define SENSOR_CENTER_MASK  (1U << 1) // A1 (PC1)
#define SENSOR_LEFT_MASK    (1U << 2) // A2 (PC2)
#define SENSOR_ALL_MASK     0x07U     // 0b00000111
void HAL_Sensor_Init(void);
uint8_t HAL_Sensor_ReadLine(void);
void HAL_Sensor_MockSet(uint8_t fake_bits);
#endif