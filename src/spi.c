#include "spi.h"
#include <stdint.h>

/* Base address for your SPI peripheral */
#define SPI_BASE       0x10001000UL
#define SPI_CTRL       (*(volatile uint32_t *)(SPI_BASE + 0x00))
#define SPI_STATUS     (*(volatile uint32_t *)(SPI_BASE + 0x04))
#define SPI_DATA       (*(volatile uint32_t *)(SPI_BASE + 0x08))

/* Control/status bits (example) */
#define SPI_EN         (1 << 0)
#define SPI_TX_READY   (1 << 1)
#define SPI_RX_VALID   (1 << 2)

void spi_init(void) {
    /* Example: enable SPI, mode 0, clock divider = CPUCLK/4 */
    SPI_CTRL = SPI_EN | (0 << 1) /* CPOL=0,CPHA=0 */ | (2 << 4);
}

void spi_transfer(uint8_t *tx, uint8_t *rx, size_t len) {
    for (size_t i = 0; i < len; i++) {
        /* Wait until TX ready */
        while (!(SPI_STATUS & SPI_TX_READY));
        SPI_DATA = tx ? tx[i] : 0xFF;
        
        /* Wait until RX valid */
        while (!(SPI_STATUS & SPI_RX_VALID));
        if (rx) {
            rx[i] = SPI_DATA;
        } else {
            (void)SPI_DATA;
        }
    }
}
// In spi_init():
    spi_tx_buf.head = spi_tx_buf.tail = 0;
    spi_rx_buf.head = spi_rx_buf.tail = 0;

// Add IRQ handler to src/spi.c:
void SPI_IRQHandler(void) {
    uint32_t st = SPI_STATUS;
    if (st & SPI_RX_VALID) {
        uint8_t b = SPI_DATA;
        uint16_t n = (spi_rx_buf.head + 1) & (SPI_BUF_SIZE - 1);
        if (n != spi_rx_buf.tail) {
            spi_rx_buf.buffer[spi_rx_buf.head] = b;
            spi_rx_buf.head = n;
        }
    }
    if (st & SPI_TX_READY && spi_tx_buf.tail != spi_tx_buf.head) {
        SPI_DATA = spi_tx_buf.buffer[spi_tx_buf.tail];
        spi_tx_buf.tail = (spi_tx_buf.tail + 1) & (SPI_BUF_SIZE - 1);
    }
}
