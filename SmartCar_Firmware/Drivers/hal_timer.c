#include "hal_timer.h"
#include <avr/interrupt.h>
#include <avr/io.h>

static volatile uint32_t sys_ticks = 0;
ISR(TIMER0_COMPA_vect) {
    sys_ticks++;
}

void HAL_Timer_Init(void) {
    uint8_t sreg = SREG;
    cli();
    TCCR0A = (1U << WGM01);
    TCCR0B = 0U;
    TCNT0 = 0U;
    OCR0A = 249U;
    TIFR0 = (1U << OCF0A);
    TIMSK0 = (1U << OCIE0A);
    TCCR0B |= (1U << CS01) | (1U << CS00);
    SREG = sreg;
}

uint32_t HAL_Timer_GetTicks(void) {
    uint32_t ticks;
    uint8_t sreg = SREG;
    cli();
    ticks = sys_ticks;
    SREG = sreg;
    return ticks;
}
bool HAL_Timer_HasElapsed(uint32_t start_tick, uint32_t period_ms) {
    return (HAL_Timer_GetTicks() - start_tick) >= period_ms;
}