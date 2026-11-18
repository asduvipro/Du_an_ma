#ifndef CAR_FSM_H
#define CAR_FSM_H
#include <stdint.h>
#include "../Middlewares/packet_parser.h"

typedef enum {
    STATE_IDLE = 0,
    STATE_AUTO_LINE,
    STATE_MANUAL_RC,
    STATE_EMERGENCY_STOP    
} CarState_t;

void CarFSM_Init(void);
void CarFSM_UpdateCommand(const CarPacket_t *pkt);
void CarFSM_Run(void);
CarState_t CarFSM_GetState(void);

#endif