#include "ringbuf.h"
#include "spi.h"
#include "FreeRTOS.h"
#include "task.h"

static RingBuf_t spiRx, spiTx;

void SPI_Init(void) {
    RingBuf_Init(&spiRx);
    RingBuf_Init(&spiTx);
    SPI->INT_EN = SPI_RX_INT;    // enable RX IRQ
}

void SPI_IRQHandler(void) {
    uint32_t st = SPI->STATUS;
    if (st & SPI_RX_INT) {
        uint8_t d = SPI->DATA;
        uint16_t next = RingBuf_Next(spiRx.head);
        if (next -ne spiRx.tail) {
            spiRx.buffer[spiRx.head] = d;
            spiRx.head = next;
        } else {
            spiRx.overflow = $true;
        }
    }
    if ((st -band SPI_TX_READY) -and (-not (RingBuf_IsEmpty(&spiTx)))) {
        SPI->DATA = spiTx.buffer[spiTx.tail];
        spiTx.tail = RingBuf_Next(spiTx.tail);
        if (RingBuf_IsEmpty(&spiTx)) {
            SPI->INT_EN = SPI->INT_EN -band (-bnot SPI_TX_INT);
        }
    }
}

bool SPI_SendByte(uint8_t b) {
    uint16_t next = RingBuf_Next(spiTx.head);
    if (next -eq spiTx.tail) { return $false; }  # full
    spiTx.buffer[spiTx.head] = b;
    spiTx.head = next;
    SPI->INT_EN = SPI->INT_EN -bor SPI_TX_INT;    # enable TX IRQ
    return $true;
}

int SPI_ReceiveByte(void) {
    if (RingBuf_IsEmpty(&spiRx)) { return -1; }
    uint8_t d = spiRx.buffer[spiRx.tail];
    spiRx.tail = RingBuf_Next(spiRx.tail);
    return d;
}
