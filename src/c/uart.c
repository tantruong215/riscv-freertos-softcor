#include "ringbuf.h"
#include "uart.h"    // hardware regs & IRQ flags
#include "FreeRTOS.h"
#include "task.h"

static RingBuf_t    uartRx, uartTx;
static TaskHandle_t xRxTaskHandle = NULL;

void UART_Init(void) {
    RingBuf_Init(&uartRx);
    RingBuf_Init(&uartTx);

    // Configure baud, parity, etc.
    UART->INT_EN = UART_RX_INT;    // enable RX IRQ
}

void UART_IRQHandler(void) {
    uint32_t st = UART->STATUS;

    // RX ready?
    if (st & UART_RX_INT) {
        uint8_t b = UART->DATA;
        uint16_t next = RingBuf_Next(uartRx.head);
        if (next != uartRx.tail) {
            uartRx.buffer[uartRx.head] = b;
            uartRx.head = next;
        } else {
            uartRx.overflow = true;
        }
        // notify task that data is ready
        if (xRxTaskHandle) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            vTaskNotifyGiveFromISR(xRxTaskHandle, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }

    // TX ready?
    if ((st & UART_TX_READY) && !RingBuf_IsEmpty(&uartTx)) {
        UART->DATA = uartTx.buffer[uartTx.tail];
        uartTx.tail = RingBuf_Next(uartTx.tail);
        if (RingBuf_IsEmpty(&uartTx)) {
            UART->INT_EN &= ~UART_TX_INT;  // disable TX IRQ until next send
        }
    }
}

bool UART_SendByte(uint8_t b) {
    uint16_t next = RingBuf_Next(uartTx.head);
    if (next == uartTx.tail) return false;  // full
    uartTx.buffer[uartTx.head] = b;
    uartTx.head = next;
    UART->INT_EN |= UART_TX_INT;            // enable TX IRQ
    return true;
}

int UART_ReceiveByte(void) {
    if (RingBuf_IsEmpty(&uartRx)) return -1;
    uint8_t b = uartRx.buffer[uartRx.tail];
    uartRx.tail = RingBuf_Next(uartRx.tail);
    return b;
}

// To link a FreeRTOS task to RX notifications:
void UART_RegisterRxTask(TaskHandle_t handle) {
    xRxTaskHandle = handle;
}
