    .section .text
    .align 2

    .global _start
_start:
    csrr  t0, mcause
    li    t1, 7             /* Machine-timer exception code */
    beq   t0, t1, handle_mtimer
    /* other exception vectors could go here */
    j     exit_trap

handle_mtimer:
    call  MachineTimer_IRQHandler
    mret

exit_trap:
    mret
