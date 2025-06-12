#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
#include "memmap.h"   /* for MTIME and MTIMECMP */

static void prvSetupTimerInterrupt(void);

/* Called by main to start the scheduler */
BaseType_t xPortStartScheduler(void)
{
    prvSetupTimerInterrupt();
    vPortStartFirstTask();
    return 0;
}

/* Configure the machine-timer to generate the RTOS tick */
static void prvSetupTimerInterrupt(void)
{
    const uint64_t ticks = configCPU_CLOCK_HZ / configTICK_RATE_HZ;
    MTIMECMP = MTIME + ticks;

    /* Enable machine-timer interrupt (bit 7 in MIE) */
    __asm volatile("csrsi mie, %0" :: "r"(1 << 7));
    /* Globally enable interrupts (MSTATUS.MIE bit) */
    __asm volatile("csrsi mstatus, %0" :: "i"(1 << 3));
}

/* Called on each tick—reschedules if higher-priority task is ready */
void FreeRTOS_Tick_Handler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    const uint64_t ticks = configCPU_CLOCK_HZ / configTICK_RATE_HZ;
    MTIMECMP += ticks;

    if (xTaskIncrementTick() != pdFALSE)
    {
        xHigherPriorityTaskWoken = pdTRUE;
    }
    if (xHigherPriorityTaskWoken != pdFALSE)
    {
        portYIELD();
    }
}

/* Trigger a context switch from software */
void vPortYield(void)
{
    __asm volatile("ecall" ::: "memory");
}

/* Not supported */
void vPortEndScheduler(void)
{
    for( ;; );
}
