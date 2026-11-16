#include "hal_sensor.h"
#include <stdint.h>
#ifdef __AVR_ATmega328P__
    #include <avr/io.h>
#else
    #include <stdio.h>
    static uint8_t mock_sensor_state = SENSOR_CENTER_MASK;
#endif
void HAL_Sensor_Init(void) {
#ifdef __AVR_ATmega328P__
    DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2));
#else
    printf("[HAL_SENSOR] Da cau hinh chan PC0, PC1, PC2 (A0..A2) lam Input\n");
#endif
}

uint8_t HAL_Sensor_ReadLine(void) {
#ifdef __AVR_ATmega328P__
    return (PINC & SENSOR_ALL_MASK);
#else
    return (mock_sensor_state & SENSOR_ALL_MASK);
#endif
}

void HAL_Sensor_MockSet(uint8_t fake_bits) {
#ifndef __AVR_ATmega328P__
    mock_sensor_state = fake_bits;
#else
    (void)fake_bits;
#endif
}
