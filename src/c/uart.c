#include "uart.h"
#include "ringbuffer.h"
#include "memmap.h"
#include "FreeRTOS.h"
#include "task.h"

/* Hardware registers */
static volatile uint32_t *UART_IRQ_EN   = (uint32_t *)(UART_BASE_ADDR + UART_IRQ_OFFSET);
static volatile uint32_t *UART_STATUS   = (uint32_t *)(UART_BASE_ADDR + UART_STATUS_OFFSET);
static volatile uint8_t  *UART_DATA     = (uint8_t  *)(UART_BASE_ADDR + UART_DATA_OFFSET);
static volatile uint32_t *UART_CTRL     = (uint32_t *)(UART_BASE_ADDR + UART_CTRL_OFFSET);

/* Buffers and task handle */
RingBuffer uart_rx_buf;
RingBuffer uart_tx_buf;
static TaskHandle_t xRxTaskHandle = NULL;

void UART_Init(void)
{
    RingBuffer_Init(&uart_rx_buf, 128);
    RingBuffer_Init(&uart_tx_buf, 128);

    /* Enable RX interrupt */
    *UART_IRQ_EN |= UART_RX_INT;
}

void UART_IRQHandler(void)
{
    uint32_t st = *UART_STATUS;

    /* RX ready */
    if (st & UART_RX_INT) {
        uint8_t b = *UART_DATA;
        RingBuffer_Put(&uart_rx_buf, b);

        /* Notify the echo task */
        if (xRxTaskHandle) {
            BaseType_t woken = pdFALSE;
            vTaskNotifyGiveFromISR(xRxTaskHandle, &woken);
            portYIELD_FROM_ISR(woken);
        }
    }

    /* TX ready */
    if ((st & UART_TX_READY) && !RingBuffer_IsEmpty(&uart_tx_buf)) {
        *UART_DATA = RingBuffer_Get(&uart_tx_buf);
        if (RingBuffer_IsEmpty(&uart_tx_buf)) {
            /* Disable TX IRQ */
            *UART_IRQ_EN &= ~UART_TX_READY;
        }
    }
}

bool UART_SendByte(uint8_t byte)
{
    if (!RingBuffer_Put(&uart_tx_buf, byte)) {
        return false;
    }
    /* Enable TX IRQ to start sending */
    *UART_IRQ_EN |= UART_TX_READY;
    return true;
}

int UART_ReceiveByte(void)
{
    if (RingBuffer_IsEmpty(&uart_rx_buf)) {
        return -1;
    }
    return (int)RingBuffer_Get(&uart_rx_buf);
}

void UART_RegisterRxTask(TaskHandle_t handle)
{
    xRxTaskHandle = handle;
}
