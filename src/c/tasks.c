#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ringbuf.h"

void vUARTEchoTask(void *pv) {
    // Register this task for RX notifications
    UART_RegisterRxTask(xTaskGetCurrentTaskHandle());

    for (;;) {
        // Wait until ISR notifies of new data
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        int c;
        // Drain and echo
        while ((c = UART_ReceiveByte()) >= 0) {
            UART_SendByte((uint8_t)c);
        }
    }
}
