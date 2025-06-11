#include "uart.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Base address for your UART peripheral */
#define UART_BASE     0x10000000UL
#define UART_DATA     (*(volatile uint8_t *)(UART_BASE + 0x00))
#define UART_STATUS   (*(volatile uint8_t *)(UART_BASE + 0x04))
#define UART_CTRL     (*(volatile uint8_t *)(UART_BASE + 0x08))
#define UART_INT_EN   (1 << 0)
#define UART_TX_BUSY  (1 << 1)
#define UART_RX_READY (1 << 2)

/* Circular buffer sizes (power of two) */
#define UART_BUF_SIZE  128
static uint8_t  txBuf[UART_BUF_SIZE];
static uint8_t  rxBuf[UART_BUF_SIZE];
static uint16_t txHead = 0, txTail = 0;
static uint16_t rxHead = 0, rxTail = 0;
static SemaphoreHandle_t rxSem;

/* Forward declaration of IRQ handler */
void UART_IRQHandler(void);

void uart_init(void) {
    RingBuf_Init(&uartRx);
    RingBuf_Init(&uartTx);
    /* Create a semaphore to signal RX data */
    rxSem = xSemaphoreCreateBinary();
    
    /* Enable RX interrupt & global UART */
    UART_CTRL = UART_INT_EN;
    
    /* Hook NVIC / machine-IRQ for UART to call UART_IRQHandler */
    /* (platform-specific vector setup omitted) */
}

void uart_putc(uint8_t c) {
    uint16_t next = (txHead + 1) & (UART_BUF_SIZE - 1);
    while (next == txTail) {
        /* buffer full: wait (or drop) */
    }
    txBuf[txHead] = c;
    txHead = next;
    
    /* Enable TX interrupt */
    UART_CTRL |= UART_INT_EN;
}

uint8_t uart_getc(void) {
    /* Wait for RX semaphore from IRQ */
    xSemaphoreTake(rxSem, portMAX_DELAY);
    uint8_t c = rxBuf[rxTail];
    rxTail = (rxTail + 1) & (UART_BUF_SIZE - 1);
    return c;
}

void UART_IRQHandler(void) {
    uint8_t status = UART_STATUS;
    
    /* RX event */
    if (status & UART_RX_READY) {
        uint16_t next = (rxHead + 1) & (UART_BUF_SIZE - 1);
        if (next != rxTail) {
            rxBuf[rxHead] = UART_DATA;
            rxHead = next;
            xSemaphoreGiveFromISR(rxSem, NULL);
        }
        /* else overflow: drop */
    }
    
    /* TX event */
    if (!(status & UART_TX_BUSY)) {
        if (txHead != txTail) {
            UART_DATA = txBuf[txTail];
            txTail = (txTail + 1) & (UART_BUF_SIZE - 1);
        } else {
            /* No more data: disable TX interrupt */
            UART_CTRL &= ~UART_INT_EN;
        }
    }
    
    /* Clear IRQ flag if needed (platform-specific) */
}
void UART_IRQHandler(void) {
    uint32_t status = UART_STATUS;

    // 1. RX ready?
    if (status & UART_RX_READY) {
        uint8_t b = UART_DATA;
        uint16_t next = (uart_rx_buf.head + 1) & (RX_BUF_SIZE - 1);
        if (next != uart_rx_buf.tail) {
            uart_rx_buf.buffer[uart_rx_buf.head] = b;
            uart_rx_buf.head = uart_rx_buf.tail = 0;
uart_rx_buf.overflow = false;
uart_tx_buf.head = uart_tx_buf.tail = 0;
uart_tx_buf.overflow = false;
        }
    }

    // 2. TX ready?
    if (!(status & UART_TX_BUSY) && (uart_tx_buf.tail != uart_tx_buf.head)) {
        UART_DATA = uart_tx_buf.buffer[uart_tx_buf.tail];
        uart_tx_buf.tail = (uart_tx_buf.tail + 1) & (TX_BUF_SIZE - 1);
    }
}
// In UART_IRQHandler:
    uint16_t next = (uart_rx_buf.head + 1) & (RX_BUF_SIZE - 1);
    if (next != uart_rx_buf.tail) {
        uart_rx_buf.buffer[uart_rx_buf.head] = UART_DATA;
        uart_rx_buf.head = next;
    } else {
        uart_rx_buf.overflow = true;
    }
