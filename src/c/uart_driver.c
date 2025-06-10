#include "portmacro.h"
#include <stdint.h>

#define UART_BASE   0x10000000UL
#define UART_DATA   (*(volatile uint8_t*)(UART_BASE + 0x00))
#define UART_STATUS (*(volatile uint8_t*)(UART_BASE + 0x04))
#define UART_CTRL   (*(volatile uint8_t*)(UART_BASE + 0x08))

void uart_init(void) {
    // (if you need any divisor or ctrl setup, do it here)
    UART_CTRL = 0x01;  // enable rx/tx
}

void uart_putc(char c) {
    // wait until TX not busy
    while (UART_STATUS & 0x01);
    UART_DATA = (uint8_t)c;
}

char uart_getc(void) {
    // wait until RX valid
    while (!(UART_STATUS & 0x02));
    return (char)UART_DATA;
}
