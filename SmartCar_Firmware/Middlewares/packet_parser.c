#include "packet_parser.h"
#include <stdint.h>
#include <stdbool.h>

void Parser_Init(PacketParser_t *parser) {
    parser->idx = 0;
}

uint8_t Packet_CalcChecksum(const uint8_t *data, uint8_t len) {
    uint8_t xor_sum = 0;
    for (uint8_t i = 0; i < len; i++) {
        xor_sum ^= data[i];
    }
    return xor_sum;
}

bool Packet_Feed(PacketParser_t *parser, uint8_t byte, CarPacket_t *out_pkt) {
    if (parser->idx == 0) {
        if (byte == PACKET_HEADER) {
            parser->buf[0] = byte;
            parser->idx = 1;
        }
        return false;
    }
     parser->buf[parser->idx++] = byte;
     if (parser->idx >= PACKET_SIZE) {
        uint8_t expected_cs = Packet_CalcChecksum(parser->buf, PACKET_SIZE - 1U);
        if (expected_cs == parser->buf[PACKET_SIZE - 1U]) {
            for (uint8_t i = 0; i < PACKET_SIZE; i++) {
                out_pkt->raw[i] = parser->buf[i];
            }
            parser->idx = 0;
            return true;
        }
        uint8_t i = 1;
        while (i < PACKET_SIZE && parser->buf[i] != PACKET_HEADER) {
            i++;
        }
        uint8_t remain = (uint8_t)(PACKET_SIZE -i);
        for (uint8_t k =0; k < remain; k++) {
            parser->buf[k] = parser->buf[i + k];
        }
        parser->idx = remain;
    }
    return false;
}