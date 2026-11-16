#include "hal_motor.h"
#include <stdint.h>
#ifdef __AVR_ATmega328P__
    #include <avr/io.h>
#else
    #include <stdio.h>
#endif
void HAL_Motor_Init(void){
#ifdef __AVR_ATmega328P__
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
    DDRD |= (1 << PD4) | (1 << PD5) | (1 << PD7);
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
#else
printf("[HAL-MOTOR] Da cau hinh chan pb1,pb2(pwm) và pd4,pd5,pd7(dir)\n");
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
    if (left_speed >= 0) {
        PORTD |= (1 << PD4);
        PORTD &= ~(1 << PD5);
        OCR1A = (uint8_t)left_speed;
    } else if {
        PORTD &= ~(1 << PD4);
        PORTD |= (1 << PD5);
        OCR1A = (uint8_t)(-left_speed);
    } else {
        PORTD &= ~((1 << PD4) | (1 << PD5));
        OCR1A = 0;
    }
    if (right_speed >= 0) {
        PORTD |= (1 << PD7);
        PORTB &= ~(1 << PB0);
        OCR1B = (uint8_t)right_speed;
    } else if {
        PORTD & ~(1 << PD7);
        PORTB |= (1 << PB0);
        OCR1B = (uint8_t)(-right_speed);
    } else {
        PORTD &= ~(1 << PD7);
        PORTB &= ~(1 << PB0);
        OCR1B = 0;
    }
#else
    printf("[HAL_MOTOR] Xuat phan cung -> Banh trai: %d | Banh phai: %d\n",
           left_speed, right_speed);
#endif
}
void HAL_Motor_Stop(void) {
#ifdef __AVR_ATmega328P__
    PORTD &= ~((1 << PD4) | (1 << PD5) | (1 << PD7));
    PORTB &= ~(1 << PB0);
    OCR1A = 0;
    OCR1B = 0;
#else
    printf("[HAL_MOTOR] DUNG KHAN CAP (All motors stopped)\n");
#endif
}