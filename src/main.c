#include "FreeRTOS.h"
#include "task.h"
#include "uart_driver.h"

extern void vPortSetupTimerInterrupt(void);

void vEchoTask(void *pv) {
    uart_init();
    for (;;) {
        char c = uart_getc();
        uart_putc(c);
    }
}

int main(void) {
    vPortSetupTimerInterrupt();
    xTaskCreate(vEchoTask, "Echo", 128, NULL, 1, NULL);
    vTaskStartScheduler();
    for (;;);
}
