#ifndef HAL_UART_H
#define HAL_UART_H
#include <stdint.h>
void HAL_UART_Init(void);
void HAL_UART_SetRxCallback(void (*callback)(uint8_t received_byte));
#endif