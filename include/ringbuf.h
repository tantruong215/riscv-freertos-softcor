#ifndef RINGBUF_H
#define RINGBUF_H

#include <stdint.h>
#include <stdbool.h>

/* Must be a power of two for masking */
#define RINGBUF_SIZE    128

typedef struct {
    uint8_t        buffer[RINGBUF_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile bool     overflow;
} RingBuffer;

/* Initialize the ring buffer */
static inline void RingBuffer_Init(RingBuffer *rb) {
    rb->head     = 0;
    rb->tail     = 0;
    rb->overflow = false;
}

/* Check empty/full */
static inline bool RingBuffer_IsEmpty(const RingBuffer *rb) {
    return rb->head == rb->tail;
}
static inline bool RingBuffer_IsFull(const RingBuffer *rb) {
    return (((rb->head + 1) & (RINGBUF_SIZE - 1)) == rb->tail);
}

/* Put & get */
static inline bool RingBuffer_Put(RingBuffer *rb, uint8_t b) {
    uint16_t next = (rb->head + 1) & (RINGBUF_SIZE - 1);
    if (next == rb->tail) {
        rb->overflow = true;
        return false;
    }
    rb->buffer[rb->head] = b;
    rb->head = next;
    return true;
}
static inline uint8_t RingBuffer_Get(RingBuffer *rb) {
    if (RingBuffer_IsEmpty(rb)) {
        return 0;
    }
    uint8_t v = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) & (RINGBUF_SIZE - 1);
    return v;
}

/* Overflow flags */
static inline bool RingBuffer_HasOverflow(const RingBuffer *rb) {
    return rb->overflow;
}
static inline void RingBuffer_ClearOverflow(RingBuffer *rb) {
    rb->overflow = false;
}

#endif /* RINGBUF_H */
