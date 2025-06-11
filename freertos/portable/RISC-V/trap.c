#include "FreeRTOS.h"
#include "task.h"

void MachineTimer_IRQHandler(void) {
    uint64_t now = *MTIME;
    *MTIMECMP = now + (configCPU_CLOCK_HZ / configTICK_RATE_HZ);
    vPortSysTickHandler();
}

/* Vector table entry: */
    .word MachineTimer_IRQHandler
