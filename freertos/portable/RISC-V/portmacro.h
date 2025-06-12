#ifndef PORTMACRO_H
#define PORTMACRO_H

#include <stdint.h>
#include "FreeRTOSConfig.h"  /* for configTICK_RATE_HZ */

/* Type definitions */
#define portSTACK_TYPE    uint32_t
#define portBASE_TYPE     long
typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;

/* Architecture specifics */
#define portSTACK_GROWTH          (-1)
#define portBYTE_ALIGNMENT        8
#define portTICK_PERIOD_MS        ( 1000U / configTICK_RATE_HZ )

/* Critical sections */
#define portENTER_CRITICAL()     __asm volatile("csrci mstatus, 8" ::: "memory")
#define portEXIT_CRITICAL()      __asm volatile("csrsi mstatus, 8" ::: "memory")

/* Yield */
#define portYIELD()              __asm volatile("ecall" ::: "memory")
#define portDISABLE_INTERRUPTS() __asm volatile("csrci mstatus, 8" ::: "memory")
#define portENABLE_INTERRUPTS()  __asm volatile("csrsi mstatus, 8" ::: "memory")

/* Tick type */
#if ( configUSE_16_BIT_TICKS == 1 )
  typedef uint16_t TickType_t;
  #define portMAX_DELAY ( TickType_t )0xFFFF
#else
  typedef uint32_t TickType_t;
  #define portMAX_DELAY ( TickType_t )0xFFFFFFFFUL
#endif

/* Scheduler utilities */
extern void vPortStartFirstTask(void);
extern void vPortYield(void);

/* Map the FreeRTOS tick handler to our machine-timer IRQ */
#define FreeRTOS_Tick_Handler    MachineTimer_IRQHandler

#endif /* PORTMACRO_H */
