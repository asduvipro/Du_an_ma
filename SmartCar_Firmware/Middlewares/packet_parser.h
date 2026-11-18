#ifndef PACKET_PARSER_H
#define PACKET_PARSER_H
#include <stdint.h>
#include <stdbool.h>
#define PACKET_HEADER 0xAAU
#define PACKET_SIZE 7U
#define CMD_EN_MASK (1U << 0)
#define CMD_MODE_MASK (1U << 1)
#define CMD_BREAK_MASK (1U << 2)

typedef struct {
    uint8_t EN : 1;
    uint8_t Mode : 1;
    uint8_t BRAKE : 1;
    uint8_t GEAR : 1;
    uint8_t RESERSED : 3;
} CmdBits_t;

typedef union {
    uint8_t all;
    CmdBits_t bits;
} CmdReg_t;

typedef union {
    struct __attribute__((packed)) {
        uint8_t header;
        CmdReg_t ctrl;
        int16_t left_speed;
        int16_t right_speed;
    } field;
    uint8_t raw[PACKET_SIZE];
} CarPacket_t;

_Static_assert(sizeof(CarPacket_t) == PACKET_SIZE, "CarPacket_t phai dung 7 byte!");

typedef struct {
    uint8_t buf[PACKET_SIZE];
    uint8_t idx;
} PacketParser_t;

void Parser_Init(PacketParser_t *parser);
uint8_t Packet_CalcChecksum(const uint8_t *data, uint8_t len);
bool Packet_Parse(CarPacket_t *out_pkt, const uint8_t raw_bytes[PACKET_SIZE]);
#endif