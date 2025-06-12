#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ringbuffer.h"

/* Echo task: waits for ISR notifications, then drains & re-sends bytes */
void vUARTEchoTask(void *pv) {
    UART_RegisterRxTask(xTaskGetCurrentTaskHandle());

    for (;;) {
        /* Block until ISR tells us data arrived */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        int c;
        while ((c = UART_ReceiveByte()) >= 0) {
            UART_SendByte((uint8_t)c);
        }
    }
}
