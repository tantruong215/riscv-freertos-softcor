#ifndef PORTMACRO_H
#define PORTMACRO_H

#include <stdint.h>

#define portCHAR        char
#define portFLOAT       float
#define portDOUBLE      double
#define portLONG        long
#define portSHORT       short
#define portSTACK_TYPE  uint32_t
#define portBASE_TYPE   long

typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;

#define portSTACK_GROWTH            ( -1 )
#define portTICK_PERIOD_MS          ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT          8

#define portENTER_CRITICAL()     __asm volatile( "csrci mstatus, 8" ::: "memory" )
#define portEXIT_CRITICAL()      __asm volatile( "csrsi mstatus, 8" ::: "memory" )
#define portYIELD()              __asm volatile( "ecall" ::: "memory" )
#define portDISABLE_INTERRUPTS() __asm volatile( "csrci mstatus, 8" ::: "memory" )
#define portENABLE_INTERRUPTS()  __asm volatile( "csrsi mstatus, 8" ::: "memory" )

#if ( configUSE_16_BIT_TICKS == 1 )
    typedef uint16_t TickType_t;
    #define portMAX_DELAY ( TickType_t ) 0xffff
#else
    typedef uint32_t TickType_t;
    #define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif

extern void vPortStartFirstTask( void );
extern void vPortYield( void );
#define FreeRTOS_Tick_Handler MachineTimer_IRQHandler

#endif /* PORTMACRO_H */
