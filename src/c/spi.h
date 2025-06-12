#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <stdbool.h>
#include "ringbuffer.h"

/* Adjust base address to match my FPGA mapping */
#define SPI_BASE_ADDR      0x10030000U
#define SPI_IRQ_OFFSET     0x00
#define SPI_STATUS_OFFSET  0x04
#define SPI_DATA_OFFSET    0x08
#define SPI_CTRL_OFFSET    0x0C

/* Bits in STATUS */
#define SPI_RX_READY       (1U << 0)
#define SPI_TX_READY       (1U << 1)
/* Bit in CTRL to start TX */
#define SPI_CTRL_TX_ENABLE (1U << 0)

/* My ring buffers for SPI RX/TX */
extern RingBuffer spi_rx_buf;
extern RingBuffer spi_tx_buf;

/* Initialize the SPI peripheral and its IRQs */
void SPI_Init(void);

/* Called from the IRQ vector when SPI IRQ fires */
void SPI_IRQHandler(void);

/* Enqueue a byte to send; returns false on overflow */
bool SPI_SendByte(uint8_t byte);

/* Dequeue a received byte, or return -1 if none */
int  SPI_ReceiveByte(void);

#endif /* SPI_H */
