#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* System clock and RTOS tick */
#define configCPU_CLOCK_HZ               ( 50000000U )   /* 50 MHz system clock */
#define configTICK_RATE_HZ               ( 1000U )       /* 1 kHz tick interrupt */

/* Heap and priorities */
#define configTOTAL_HEAP_SIZE            ( 2048U )
#define configMAX_PRIORITIES             ( 4 )

/* Feature enablement */
#define configUSE_PREEMPTION             1
#define configUSE_IDLE_HOOK              0
#define configUSE_TICK_HOOK              0
#define configSUPPORT_DYNAMIC_ALLOCATION 1
#define configSUPPORT_STATIC_ALLOCATION  0
#define configMINIMAL_STACK_SIZE         ( 128 )

/* Use the RISC-V portable layer */
#include "portable/RISC-V/portmacro.h"

#endif /* FREERTOS_CONFIG_H */
