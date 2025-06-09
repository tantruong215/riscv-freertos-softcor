// include/FreeRTOSConfig.h
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configCPU_CLOCK_HZ        (50000000U)
#define configTICK_RATE_HZ        (1000U)
#define configTOTAL_HEAP_SIZE     (2048U)
#define configMAX_PRIORITIES      (4)
#define configUSE_PREEMPTION      1
#define configUSE_IDLE_HOOK       0
#define configUSE_TICK_HOOK       0
#define configMINIMAL_STACK_SIZE  (128)
#define configSUPPORT_STATIC_ALLOCATION 0
#define configSUPPORT_DYNAMIC_ALLOCATION 1

#include "freertos/portable/RISC-V/portmacro.h"

#endif // FREERTOS_CONFIG_H
