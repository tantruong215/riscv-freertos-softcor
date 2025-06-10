#include "uart.h"

/* Match these to the base-address your top-level assigns. */
#define UART_BASE    0x10000000UL
#define UART_DATA    (*(volatile uint8_t*)(UART_BASE + 0x00))
#define UART_STATUS  (*(volatile uint8_t*)(UART_BASE + 0x04))
#define UART_CTRL    (*(volatile uint8_t*)(UART_BASE + 0x08))

void uart_init(void) {
    /* Example: enable RX and TX (your CTRL bit-fields may differ) */
    UART_CTRL = 0x01;
}

void uart_putc(uint8_t c) {
    /* Wait until TX is free (status bit 0 == 0) */
    while (UART_STATUS & 0x01) { }
    UART_DATA = c;
}

uint8_t uart_getc(void) {
    /* Wait until RX ready (status bit 1 == 1) */
    while (!(UART_STATUS & 0x02)) { }
    return UART_DATA;
}
