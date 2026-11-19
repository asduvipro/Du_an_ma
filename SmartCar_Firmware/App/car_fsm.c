#include "car_fsm.h"
#include "../Drivers/hal_motor.h"
#include "../Drivers/hal_sensor.h"
#include "../Drivers/hal_timer.h"
#include <stddef.h>
#include <stdint.h>
#define RC_FAILSAFE_TIMEOUT_MS 500U
#define LINE_LOST_TIMEOUT_MS   250U

static CarState_t current_state = STATE_IDLE;
static CarPacket_t active_cmd;
static uint32_t last_cmd_tick = 0U;
static int8_t last_dir = 0;
static uint32_t line_lost_tick = 0U;
static bool is_line_lost = false;
static const int16_t GEAR_SPEED_TABLE[4] = {120, 155, 190, 230};
void CarFSM_Init(void) {
    current_state = STATE_IDLE;
    active_cmd.field.ctrl.all = 0U;
    active_cmd.field.left_speed = 0;
    active_cmd.field.right_speed = 0;

    last_cmd_tick = HAL_Timer_GetTicks();
    last_dir = 0;
    line_lost_tick = 0U;
    is_line_lost = false;
    HAL_Motor_Coast();
}

void CarFSM_UpdateCommand(const CarPacket_t *pkt) {
    if (pkt == NULL) {
        return;
    }
    active_cmd = *pkt;
    last_cmd_tick =HAL_Timer_GetTicks();
    if (active_cmd.field.ctrl.bits.BRAKE == 1U) {
        current_state = STATE_EMERGENCY_STOP;
        return;
    }
    if (active_cmd.field.ctrl.bits.EN == 0U) {
        current_state = STATE_IDLE;
        return;
    }
    if (active_cmd.field.ctrl.bits.Mode == 0U) {
        if (current_state != STATE_AUTO_LINE) {
            last_dir = 0;
            is_line_lost = false;
        }
        current_state = STATE_AUTO_LINE;
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
            if (HAL_Timer_HasElapsed(last_cmd_tick, RC_FAILSAFE_TIMEOUT_MS)) {
                current_state = STATE_IDLE;
                HAL_Motor_Coast();
                break;
            }
            
            HAL_Motor_SetSpeed(active_cmd.field.left_speed, active_cmd.field.right_speed);
            break;
        case STATE_AUTO_LINE: {
            uint8_t sensor = HAL_Sensor_ReadLine();
            uint16_t base_spd = GEAR_SPEED_TABLE[active_cmd.field.ctrl.bits.GEAR];
            int16_t soft_turn_spd = (int16_t)((base_spd *7) / 10);
            int16_t sharp_turn_spd = (int16_t)((base_spd *3) / 10);
            if (sensor != 0U) {
                is_line_lost = false;
            }   
            switch (sensor) {
                case SENSOR_CENTER_MASK:
                    last_dir = 0;
                    HAL_Motor_SetSpeed(base_spd, base_spd);
                    break;
                case (SENSOR_LEFT_MASK | SENSOR_CENTER_MASK):
                    last_dir = -1;
                    HAL_Motor_SetSpeed(soft_turn_spd, base_spd);
                    break;

                case SENSOR_LEFT_MASK:
                    last_dir = -1;
                    HAL_Motor_SetSpeed(sharp_turn_spd, base_spd);
                    break;

                case (SENSOR_CENTER_MASK | SENSOR_RIGHT_MASK):
                    last_dir = 1;
                    HAL_Motor_SetSpeed(base_spd, soft_turn_spd);
                    break;

                case SENSOR_RIGHT_MASK:
                    last_dir = 1;
                    HAL_Motor_SetSpeed(base_spd, sharp_turn_spd);
                    break;

                case SENSOR_ALL_MASK:
                    case (SENSOR_LEFT_MASK | SENSOR_RIGHT_MASK):
                        HAL_Motor_SetSpeed(base_spd, base_spd);
                        break;
                    case 0U:
                    default:
                        if (!is_line_lost) {
                            is_line_lost   = true;
                            line_lost_tick = HAL_Timer_GetTicks();
                        }

                        if (HAL_Timer_HasElapsed(line_lost_tick, LINE_LOST_TIMEOUT_MS) || (last_dir == 0)) {
                            HAL_Motor_Brake();
                        } else if (last_dir < 0) {
                            HAL_Motor_SetSpeed(0, base_spd); // Quay đầu sang trái tìm lại vạch
                        } else {
                            HAL_Motor_SetSpeed(base_spd, 0); // Quay đầu sang phải tìm lại vạch
                        }
                        break;
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