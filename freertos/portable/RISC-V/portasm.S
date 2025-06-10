    .section .text."vPortStartFirstTask"
    .global vPortStartFirstTask
vPortStartFirstTask:
    la   t0, pxCurrentTCB
    lw   t0, 0(t0)
    lw   t0, 0(t0)
    mv   sp, t0
    lw   ra,  0(sp); addi sp, sp, 4
    lw   t0,  0(sp); addi sp, sp, 4
    lw   t1,  0(sp); addi sp, sp, 4
    lw   t2,  0(sp); addi sp, sp, 4
    lw   t3,  0(sp); addi sp, sp, 4
    lw   t4,  0(sp); addi sp, sp, 4
    lw   t5,  0(sp); addi sp, sp, 4
    lw   t6,  0(sp); addi sp, sp, 4
    lw   a0,  0(sp); addi sp, sp, 4
    lw   a1,  0(sp); addi sp, sp, 4
    lw   a2,  0(sp); addi sp, sp, 4
    lw   a3,  0(sp); addi sp, sp, 4
    lw   a4,  0(sp); addi sp, sp, 4
    lw   a5,  0(sp); addi sp, sp, 4
    lw   a6,  0(sp); addi sp, sp, 4
    lw   a7,  0(sp); addi sp, sp, 4
    lw   t3,  0(sp); addi sp, sp, 4
    csrw mepc, t3
    lw   t3,  0(sp); addi sp, sp, 4
    csrw mstatus, t3
    addi sp, sp, 4
    mret
