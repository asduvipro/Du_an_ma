#include "car_fsm.h"
#include "../Drivers/hal_motor.h"
#include "../Drivers/hal_sensor.h"
#include <stddef.h>
#include <stdint.h>

static CarState_t current_state = STATE_IDLE;
static CarPacket_t active_cmd;
static const int16_t GEAR_SPEED_TABLE[4] = {100, 140, 180, 220};
void CarFSM_Init(void) {
    current_state = STATE_IDLE;
    active_cmd.field.ctrl.all = 0;
    active_cmd.field.left_speed = 0;
    active_cmd.field.right_speed = 0;
    HAL_Motor_Coast();
}

void CarFSM_UpdateCommand(const CarPacket_t *pkt) {
    if (pkt == NULL) {
        return;
    }
    active_cmd = *pkt;
    if (active_cmd.field.ctrl.bits.BRAKE == 1U) {
        current_state = STATE_EMERGENCY_STOP;
        return;
    }
    if (active_cmd.field.ctrl.bits.EN == 0U) {
        current_state = STATE_IDLE;
        return;
    }
    if (active_cmd.field.ctrl.bits.Mode == 0U) {
        current_state = STATE_AUTO_LINE;
        return;
    } else {
        current_state = STATE_MANUAL_RC;
    }
}

void CarFSM_Run(void) {
    switch (current_state) {
        case STATE_IDLE:
            HAL_Motor_Coast();
            break;
        case STATE_EMERGENCY_STOP:
            HAL_Motor_Brake();
            break;
        case STATE_MANUAL_RC:
            HAL_Motor_SetSpeed(active_cmd.field.left_speed, active_cmd.field.right_speed);
            break;
        case STATE_AUTO_LINE: {
            uint8_t sensor = HAL_Sensor_ReadLine();
            uint16_t base_spd = GEAR_SPEED_TABLE[active_cmd.field.ctrl.bits.GEAR];
            int16_t turn_spd = (int16_t)(base_spd / 3);
            if (sensor == SENSOR_CENTER_MASK || sensor == SENSOR_ALL_MASK) {
                HAL_Motor_SetSpeed(base_spd, base_spd);
            } else if (sensor & SENSOR_LEFT_MASK) {
                HAL_Motor_SetSpeed(turn_spd, base_spd);
            } else if (sensor & SENSOR_RIGHT_MASK) {
                HAL_Motor_SetSpeed(base_spd, turn_spd);
            } else {
                HAL_Motor_Brake();
            }
            break;
        }   
        default:
            HAL_Motor_Coast();
            break;
    }
}
CarState_t CarFSM_GetState(void) {
    return current_state;
}