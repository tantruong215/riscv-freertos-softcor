#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"

/* Provided by your FreeRTOS port */
extern void vPortSetupTimerInterrupt(void);

void vLedTask(void *pvParameters) {
    led_init();
    for (;;) {
        led_toggle();
        vTaskDelay(pdMS_TO_TICKS(500));  // 500 ms blink
    }
}

void vEchoTask(void *pvParameters) {
    uart_init();
    for (;;) {
        uint8_t c = uart_getc();
        uart_putc(c);
    }
}

int main(void) {
    /* Kick off the 1 kHz RTOS tick */
    vPortSetupTimerInterrupt();

    /* Create demo tasks */
    xTaskCreate(vLedTask,  "LED",  128, NULL, 1, NULL);
    xTaskCreate(vEchoTask, "Echo", 128, NULL, 1, NULL);

    /* Start the scheduler */
    vTaskStartScheduler();

    /* Should never reach here */
    for (;;);
    return 0;
}
