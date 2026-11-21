#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

#include "hal_motor.h"
#include "hal_sensor.h"
#include "hal_uart.h"
#include "hal_timer.h"
#include "ring_buffer.h"
#include "packet_parser.h"
#include "car_fsm.h"

#define STANDALONE_MODE 1
#define CONTROL_LOOP_MS 10U
#define RX_BUF_SIZE 64U
static RingBuffer_t rx_rb;
static PacketParser_t cmd_parser;
static void OnUARTByteReceived(uint8_t received_byte) {
    (void)RingBuffer_Put(&rx_rb, received_byte);
}

static void StandaloneSwitch_Init(void) {
    DDRD &= ~(1U << PD2);
    PORTD |= (1U << PD2);
}

static bool __attribute__((unused)) StandaloneSwitch_IsRunPressed(void) {
    return (PIND & (1U << PD2)) == 0U;
}

int main(void) {
    RingBuffer_Init(&rx_rb);
    Parser_Init(&cmd_parser);

    HAL_Motor_Init();
    HAL_Sensor_Init();
    HAL_UART_Init();
    HAL_UART_SetRxCallback(OnUARTByteReceived);
    HAL_Timer_Init();
    StandaloneSwitch_Init();

    CarFSM_Init();
    sei();

    uint32_t last_control_tick = HAL_Timer_GetTicks();
    uint8_t rx_byte;
    CarPacket_t parser_pkt;

    while(1) {
        while (RingBuffer_Get(&rx_rb, &rx_byte)) {
            if (Packet_Feed(&cmd_parser, rx_byte, &parser_pkt)) {
                CarFSM_UpdateCommand(&parser_pkt);
            }
        }
#if (STANDALONE_MODE == 1) 
        CarPacket_t local_cmd;
        local_cmd.field.ctrl.all = 0U;
        local_cmd.field.ctrl.bits.EN = StandaloneSwitch_IsRunPressed() ? 1U : 0U;
        local_cmd.field.ctrl.bits.Mode = 0U;
        local_cmd.field.ctrl.bits.GEAR = 1U;
        local_cmd.field.left_speed = 0;
        local_cmd.field.right_speed = 0;
        CarFSM_UpdateCommand(&local_cmd);
#endif

        if (HAL_Timer_HasElapsed(last_control_tick, CONTROL_LOOP_MS)) {
            last_control_tick = HAL_Timer_GetTicks();
            CarFSM_Run();
        }
    }

    return 0;
}