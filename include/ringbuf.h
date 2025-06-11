#ifndef RINGBUF_H
#define RINGBUF_H

#include <stdint.h>
#include <stdbool.h>

#define RINGBUF_SIZE_PWR2 128       // must be a power of two

typedef struct {
    uint8_t  buffer[RINGBUF_SIZE_PWR2];
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile bool     overflow;
} RingBuf_t;

static inline void RingBuf_Init(RingBuf_t *rb) {
    rb->head = rb->tail = 0;
    rb->overflow = false;
}

static inline uint16_t RingBuf_Next(uint16_t idx) {
    return (idx + 1) & (RINGBUF_SIZE_PWR2 - 1);
}

static inline bool RingBuf_IsEmpty(const RingBuf_t *rb) {
    return rb->head == rb->tail;
}

static inline bool RingBuf_IsFull(const RingBuf_t *rb) {
    return RingBuf_Next(rb->head) == rb->tail;
}

#endif // RINGBUF_H
