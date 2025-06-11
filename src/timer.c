#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

#define MTIME       (*(volatile uint64_t *)0x0200BFF8UL)
#define MTIMECMP    (*(volatile uint64_t *)0x02004000UL)
#define TICK_DELTA  ((configCPU_CLOCK_HZ) / (configTICK_RATE_HZ))

void MachineTimer_IRQHandler(void)
{
    uint64_t now = MTIME;
    MTIMECMP = now + TICK_DELTA;    // schedule next tick

    /* Call the FreeRTOS tick handler */
    vPortSysTickHandler();
}
