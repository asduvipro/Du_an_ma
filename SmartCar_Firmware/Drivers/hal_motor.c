#include "hal_motor.h"
#include <stdint.h>
#ifdef __AVR_ATmega328P__
    #include <avr/io.h>
#else
    #include <stdio.h>
#endif

void HAL_Motor_Init(void){
#ifdef __AVR_ATmega328P__
    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
    DDRD |= (1 << PD3);
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
    TCCR2A = (1 << COM1A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << CS22);
    HAL_Motor_Coast();
#else
printf("[HAL-MOTOR] Khoi tao DRV8833 (D9, D10, D11, D3)\n");
#endif
}

void HAL_Motor_SetSpeed(int16_t left_speed, int16_t right_speed){
    if (left_speed > 255) {
        left_speed = 255;
    } else if (left_speed < -255) {
        left_speed = -255;
    }
    if (right_speed > 255) {
        right_speed = 255;
    } else if (right_speed < -255) {
        right_speed = -255;
    }
#ifdef __AVR_ATmega328P__
    if (left_speed > 0) {
        OCR1A = (uint8_t)left_speed;
        OCR1B = 0;
    } else if (left_speed < 0) {
        OCR1A = 0;
        OCR1B = (uint8_t)(-left_speed);
    } else {
        OCR1A = 0;
        OCR1B = 0;
    }
    if (right_speed > 0) {
        OCR2A = (uint8_t)right_speed;
        OCR2B = 0;
    } else if (right_speed < 0) {
        OCR2A - 0;
        OCR2B = (uint8_t)(-right_speed);
    } else {
        OCR2A = 0;
        OCR2B = 0;
    }
#else
    printf("[HAL_MOTOR] Xuat phan cung -> Banh trai: %d | Banh phai: %d\n",
           left_speed, right_speed);
#endif
}

void HAL_Motor_Coast(void) {
#ifdef __AVR_ATmega328P__
    OCR1A = 0;
    OCR1B = 0;
    OCR2A = 0;
    OCR2B = 0;
#else
    printf("[HAL_MOTOR] DRV8833 -> THA TROI (Coast)\n");
#endif
}

void HAL_Motor_Brake(void) {
#ifdef __AVR_ATmega328P__
    // Cả 4 chân kéo lên 100% (255) -> Nối tắt 2 cực động cơ (Phanh hãm điện từ)
    OCR1A = 255;
    OCR1B = 255;
    OCR2A = 255;
    OCR2B = 255;
#else
    printf("[HAL_MOTOR] DRV8833 -> PHANH GAP (Brake)\n");
#endif
}