#include "packet_parser.h"
#include <stdint.h>
#include<stdbool.h>

bool Packet_Parse(CarPacket_t *out_pkt, const uint8_t raw_bytes[PACKET_SIZE]) {
    for (uint8_t i = 0; i < PACKET_SIZE; i++) {
        out_pkt->raw[i] = raw_bytes[i];
    }
    if (out_pkt->field.header == PACKET_HEADER) {
        return true;
    }
    return false;
}