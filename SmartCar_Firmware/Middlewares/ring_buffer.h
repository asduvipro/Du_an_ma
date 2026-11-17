#ifndef RING_BUFFER_H
#define RING_BUFFER_H
#include <stdint.h>
#include <stdbool.h>
#define RB_SIZE 16U
#define RB_MASK (RB_SIZE -1U)
typedef struct {
    uint8_t buffer[RB_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
} RingBuffer_t;

void RingBuffer_Init(RingBuffer_t *rb);
bool RingBuffer_Put(RingBuffer_t *rb, uint8_t data);
bool RingBuffer_Get(RingBuffer_t *rb, uint8_t *p_data);

#endif