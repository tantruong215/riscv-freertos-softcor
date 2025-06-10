#include \"freertos/FreeRTOS.h\"
#include \"freertos/task.h\"
#include \"driver/gpio.h\"

extern void vPortSetupTimerInterrupt(void);

void vLedTask(void *p) {
    for (;;) {
        gpio_toggle(LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {
    vPortSetupTimerInterrupt();
    xTaskCreate(vLedTask, \"LED\", 128, NULL, 1, NULL);
    vTaskStartScheduler();
    for (;;);
}
