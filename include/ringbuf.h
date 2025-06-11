#ifndef RINGBUF_H
#define RINGBUF_H

#include <stdint.h>
#include <stdbool.h>

#define UART_BUF_SIZE 128

typedef struct {
    uint8_t  buffer[UART_BUF_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile bool     overflow;
} RingBuf_t;

/* Initialize buffer */
static inline void RingBuf_Init(RingBuf_t *rb) {
    rb->head = rb->tail = 0;
    rb->overflow = false;
}

/* Advance an index with wrap (power-of-2 size) */
static inline uint16_t RingBuf_Next(uint16_t idx) {
    return (idx + 1) & (UART_BUF_SIZE - 1);
}

/* Check empty/full */
static inline bool RingBuf_IsEmpty(const RingBuf_t *rb) {
    return rb->head == rb->tail;
}
static inline bool RingBuf_IsFull(const RingBuf_t *rb) {
    return RingBuf_Next(rb->head) == rb->tail;
}

#endif // RINGBUF_H
