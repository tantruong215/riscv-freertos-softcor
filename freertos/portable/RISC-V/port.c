#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"

#define CLINT_BASE       0x02000000UL
#define MTIMECMP_OFFSET  0x4000
#define MTIME_OFFSET     0xBFF8UL

volatile uint64_t * const mtime    = (uint64_t*)(CLINT_BASE + MTIME_OFFSET);
volatile uint64_t * const mtimecmp = (uint64_t*)(CLINT_BASE + MTIMECMP_OFFSET);

void FreeRTOS_Tick_Handler( void );
static void prvSetupTimerInterrupt( void );

BaseType_t xPortStartScheduler( void )
{
    prvSetupTimerInterrupt();
    vPortStartFirstTask();
    return 0;
}

static void prvSetupTimerInterrupt( void )
{
    uint64_t ticks = configCPU_CLOCK_HZ / configTICK_RATE_HZ;
    *mtimecmp = *mtime + ticks;
    __asm volatile( "csrsi mie, 0x80" );
    __asm volatile( "csrsi mstatus, 0x8" );
}

void FreeRTOS_Tick_Handler( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint64_t ticks = configCPU_CLOCK_HZ / configTICK_RATE_HZ;
    *mtimecmp += ticks;
    if( xTaskIncrementTick() != pdFALSE )
    {
        xHigherPriorityTaskWoken = pdTRUE;
    }
    if( xHigherPriorityTaskWoken != pdFALSE )
    {
        portYIELD();
    }
}

void vPortYield( void )
{
    __asm volatile( "ecall" );
}

void vPortEndScheduler( void )
{
    for( ;; );
}
void vPortSetupTimerInterrupt(void) {
    uint64_t now = MTIME;
    MTIMECMP = now + (configCPU_CLOCK_HZ / TICK_RATE_HZ);
    __asm volatile(\"csrs mie, %0\" :: \"r\"(1 << 7));
}
/* PLIC setup */
#define PLIC_THRESHOLD      ((volatile uint32_t*)0x0C000000)
#define PLIC_PRIORITY_BASE  ((volatile uint32_t*)0x0C001000)
#define PLIC_ENABLE_BASE    ((volatile uint32_t*)0x0C002000)
void PLIC_Init(void) {
    *PLIC_THRESHOLD = 0;
    PLIC_PRIORITY_BASE[7] = 3;   // Machine-timer
    PLIC_PRIORITY_BASE[10] = 1;  // UART
    // set SPI, I2C similarly...
    PLIC_ENABLE_BASE[0] |= (1<<7) | (1<<10);  
}
