#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"   /* for configCPU_CLOCK_HZ, configTICK_RATE_HZ */
#include "memmap.h"            /* defines MTIME_BASE, MTIMECMP_BASE */

#define MTIME    (*(volatile uint64_t *)(MTIME_BASE + 0xBFF8))
#define MTIMECMP (*(volatile uint64_t *)(MTIME_BASE + 0x4000))
#define TICK_DELTA ((uint64_t)configCPU_CLOCK_HZ / configTICK_RATE_HZ)

/* Called by the trap vector on machine-timer IRQ */
void MachineTimer_IRQHandler(void)
{
    uint64_t now = MTIME;
    MTIMECMP = now + TICK_DELTA;    /* schedule next tick */
    vPortSysTickHandler();          /* FreeRTOS tick */
}
