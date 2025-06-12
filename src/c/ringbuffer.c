#include "ringbuffer.h"
#include <stdbool.h>
#include <stdlib.h>

void RingBuffer_Init(RingBuffer *rb, size_t size) {
    rb->buffer = malloc(size);
    rb->size   = size;
    rb->head   = rb->tail = 0;
    rb->overflow = false;
}

bool RingBuffer_IsEmpty(const RingBuffer *rb) {
    return rb->head == rb->tail;
}

bool RingBuffer_IsFull(const RingBuffer *rb) {
    return ((rb->head + 1) % rb->size) == rb->tail;
}

bool RingBuffer_Put(RingBuffer *rb, uint8_t byte) {
    size_t next = (rb->head + 1) % rb->size;
    if (next == rb->tail) {
        rb->overflow = true;
        return false;
    }
    rb->buffer[rb->head] = byte;
    rb->head = next;
    return true;
}

uint8_t RingBuffer_Get(RingBuffer *rb) {
    if (RingBuffer_IsEmpty(rb)) {
        return 0;  // caller should check IsEmpty first
    }
    uint8_t b = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % rb->size;
    return b;
}

bool RingBuffer_HasOverflow(const RingBuffer *rb) {
    return rb->overflow;
}

void RingBuffer_ClearOverflow(RingBuffer *rb) {
    rb->overflow = false;
}
