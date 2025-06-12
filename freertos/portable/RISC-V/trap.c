#include "FreeRTOS.h"
#include "task.h"
#include "memmap.h"

/* Machine-timer IRQ handler: reprogram next tick, then call RTOS handler */
void MachineTimer_IRQHandler(void)
{
    uint64_t now = MTIME;
    MTIMECMP = now + (configCPU_CLOCK_HZ / configTICK_RATE_HZ);
    xPortSysTickHandler();
}
