#include "uart.h"   // or a shared include
#include <stdbool.h>

bool RingBuffer_HasOverflow(RingBuffer *rb) {
    return rb->overflow;
}
void RingBuffer_ClearOverflow(RingBuffer *rb) {
    rb->overflow = false;
}
