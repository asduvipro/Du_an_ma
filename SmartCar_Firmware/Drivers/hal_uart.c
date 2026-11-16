#include "hal_uart.h"
#include <stdint.h>
#include <stddef.h>
#ifdef __AVR_ATmega328P__
    #include <avr/io.h>
    #include <avr/interrupt.h>
#else
    #include <stdio.h>
#endif
static void (*volatile rx_callback_fn)(uint8_t) = NULL;
void HAL_UART_Init(void) {
#ifdef __AVR_ATmega328P__
    UBRR0H = 0;
    UBRR0L = 103;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
#else
    printf("[HAL_UART] Da cau hinh chan PD0 (RX) toc do 9600 bps, bat ngat RX\n");
#endif 
}
void HAL_UART_SetRxCallback(void (*callback)(uint8_t received_byte)) {
    rx_callback_fn = callback;
}
#ifdef __AVR_ATmega328P__
ISR(UART_RX_vect) {
    uint8_t data = UDR0;
    if (rx_callback_fn != NULL) {
        rx_callback_fn(data);
    }
}
#endif