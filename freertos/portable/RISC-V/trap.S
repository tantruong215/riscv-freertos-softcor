    li   t0, 7
    beq  a1, t0, handle_mtimer
    ...
handle_mtimer:
    call xPortSysTickHandler
    mret
.global MachineTimer_Handler
MachineTimer_Handler:
    /* Schedule next tick */
    csrr    t1, mtime
    li      t0, configCPU_CLOCK_HZ / configTICK_RATE_HZ
    add     t1, t1, t0
    csrw    mtimecmp, t1

    /* Notify FreeRTOS of the tick */
    call    xPortSysTickHandler

    mret
    /* Machine-Timer exception slot (cause 7) */
    .align 2
    .global MachineTimer_IRQHandler
MachineTimer_IRQHandler:
    call MachineTimer_IRQHandler
    mret
