#ifndef HAL_TIMER_H
#define HAL_TIMER_H
#include <stdbool.h>
#include <stdint.h>
void HAL_Timer_Init(void);
uint32_t HAL_Timer_GetTicks(void);
bool HAL_Timer_HasElapsed(uint32_t start_tick, uint32_t period_ms);
#endif