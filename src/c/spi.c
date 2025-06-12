#include "spi.h"
#include "memmap.h"    /* defines SPI_BASE_ADDR if you prefer */
#include <stdbool.h>

/* Map registers */
static volatile uint32_t *SPI_IRQ_ENABLE = (uint32_t *)(SPI_BASE_ADDR + SPI_IRQ_OFFSET);
static volatile uint32_t *SPI_STATUS     = (uint32_t *)(SPI_BASE_ADDR + SPI_STATUS_OFFSET);
static volatile uint8_t  *SPI_DATA       = (uint8_t  *)(SPI_BASE_ADDR + SPI_DATA_OFFSET);
static volatile uint32_t *SPI_CTRL       = (uint32_t *)(SPI_BASE_ADDR + SPI_CTRL_OFFSET);

/* Define buffers */
RingBuffer spi_rx_buf;
RingBuffer spi_tx_buf;

void SPI_Init(void)
{
    RingBuffer_Init(&spi_rx_buf, 128);
    RingBuffer_Init(&spi_tx_buf, 128);

    /* Enable RX and TX interrupts */
    *SPI_IRQ_ENABLE = SPI_RX_READY | SPI_TX_READY;
}

bool SPI_SendByte(uint8_t byte)
{
    if (!RingBuffer_Put(&spi_tx_buf, byte)) {
        return false;
    }
    /* Kick off TX engine */
    *SPI_CTRL |= SPI_CTRL_TX_ENABLE;
    return true;
}

int SPI_ReceiveByte(void)
{
    if (RingBuffer_IsEmpty(&spi_rx_buf)) {
        return -1;
    }
    return (int)RingBuffer_Get(&spi_rx_buf);
}

void SPI_IRQHandler(void)
{
    uint32_t status = *SPI_STATUS;

    if (status & SPI_RX_READY) {
        uint8_t b = *SPI_DATA;
        RingBuffer_Put(&spi_rx_buf, b);
    }

    if (status & SPI_TX_READY) {
        if (!RingBuffer_IsEmpty(&spi_tx_buf)) {
            *SPI_DATA = RingBuffer_Get(&spi_tx_buf);
        } else {
            /* Disable TX when done */
            *SPI_CTRL &= ~SPI_CTRL_TX_ENABLE;
        }
    }

    /* Clear IRQ flags */
    *SPI_STATUS = status;
}
