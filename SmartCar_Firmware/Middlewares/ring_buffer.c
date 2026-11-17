#include "ring_buffer.h"
#include <stdint.h>
#include <stdbool.h>

void RingBuffer_Init(RingBuffer_t *rb) {
    rb->head = 0;
    rb->tail = 0;
}

bool RingBuffer_Put(RingBuffer_t *rb,uint8_t data) {
    uint8_t next_head = (rb->head + 1U) & RB_MASK;
    if (next_head == rb->tail) {
        return false;
    }
    rb->buffer[rb->head] = data;
    rb->head = next_head;
    return true;
}
bool RingBuffer_Get(RingBuffer_t *rb,uint8_t *p_data) {
    if (rb->head == rb->tail) {
        return false;
    }
    *p_data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1U) & RB_MASK;
    return true;
}