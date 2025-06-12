#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>
#include "ringbuffer.h"
#include "FreeRTOS.h"
#include "task.h"

/* Hardware mapping */
#define UART_BASE_ADDR    0x10000000U
#define UART_IRQ_OFFSET   0x00
#define UART_STATUS_OFFSET 0x04
#define UART_DATA_OFFSET   0x08
#define UART_CTRL_OFFSET   0x0C

/* Bits */
#define UART_RX_INT       (1U << 0)
#define UART_TX_READY     (1U << 1)

/* Our RX/TX buffers */
extern RingBuffer uart_rx_buf;
extern RingBuffer uart_tx_buf;

/* Initialize UART peripheral & interrupts */
void UART_Init(void);

/* ISR entrypoint */
void UART_IRQHandler(void);

/* Send/receive API */
bool UART_SendByte(uint8_t byte);
int  UART_ReceiveByte(void);

/* Link a FreeRTOS task to RX notifications */
void UART_RegisterRxTask(TaskHandle_t handle);

#endif /* UART_H */
