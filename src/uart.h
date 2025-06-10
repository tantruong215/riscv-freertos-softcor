#ifndef UART_H
#define UART_H

#include <stdint.h>

/**
 * @brief  Initialize the UART peripheral (baud rate, enable RX/TX).
 */
void uart_init(void);

/**
 * @brief  Transmit one character (blocks if UART busy).
 */
void uart_putc(uint8_t c);

/**
 * @brief  Receive one character (blocks until data available).
 */
uint8_t uart_getc(void);

#endif // UART_H
