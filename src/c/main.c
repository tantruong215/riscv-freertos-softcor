#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"
#include "i2c.h"
#include "spi.h"

/* Set up the trap vector & machine-timer interrupt */
extern void _vector_table(void);
static void init_traps(void)
{
    uintptr_t vt = (uintptr_t)&_vector_table | 1;  // vectored mode
    __asm volatile("csrw mtvec, %0" :: "r"(vt));

    /* Enable machine-timer interrupts in MIE (bit 7) */
    __asm volatile("csrrs zero, mie, %0" :: "r"(1 << 7));
    /* Globally enable interrupts (MSTATUS.MIE bit) */
    __asm volatile("csrsi mstatus, %0" :: "i"(1 << 3));
}

/* Configure timer tick for FreeRTOS (1 kHz) */
extern void prvSetupTimerInterrupt(void);

int main(void)
{
    init_traps();
    prvSetupTimerInterrupt();

    led_init();
    UART_Init();
    I2C_Init();
    SPI_Init();

    /* Create demo tasks, e.g.: */
    // xTaskCreate(UART_EchoTask, "UartEcho", 128, NULL, 1, NULL);
    // xTaskCreate(LED_BlinkTask, "LedBlink", 128, NULL, 1, NULL);

    vTaskStartScheduler();

    /* Should never get here */
    for (;;);
}
