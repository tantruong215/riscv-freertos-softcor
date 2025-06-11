    .section .text
    .align  2
    .global portSAVE_CONTEXT
portSAVE_CONTEXT:
    /* a0 holds pointer to current TCB */
    addi    sp,   sp,  -4*34       /* make room on stack for regs x1?x31 + mstatus */
    sw      ra,   0(sp)           /* return address */
    sw      t0,   4(sp)
    sw      t1,   8(sp)
    sw      t2,   12(sp)
    sw      t3,   16(sp)
    sw      t4,   20(sp)
    sw      t5,   24(sp)
    sw      t6,   28(sp)
    sw      a0,   32(sp)          /* pointer to TCB (we'll overwrite this slot below) */
    /* Save remaining registers into the stack frame */
    sw      a1,   36(sp)
    sw      a2,   40(sp)
    sw      a3,   44(sp)
    sw      a4,   48(sp)
    sw      a5,   52(sp)
    sw      a6,   56(sp)
    sw      a7,   60(sp)
    sw      s0,   64(sp)
    sw      s1,   68(sp)
    sw      s2,   72(sp)
    sw      s3,   76(sp)
    sw      s4,   80(sp)
    sw      s5,   84(sp)
    sw      s6,   88(sp)
    sw      s7,   92(sp)
    sw      s8,   96(sp)
    sw      s9,   100(sp)
    sw      s10,  104(sp)
    sw      s11,  108(sp)
    /* Save mstatus (CSR) */
    csrr    t0, mstatus
    sw      t0,   112(sp)
    /* Store the updated stack pointer back into the TCB */
    lw      t1,   32(sp)          /* reload original a0 (TCB pointer) */
    sw      sp,   0(t1)           /* *(TCB)->sp = sp */
    ret

    .global portRESTORE_CONTEXT
portRESTORE_CONTEXT:
    /* a0 holds pointer to next TCB */
    lw      sp,   0(a0)           /* sp = *(TCB)->sp */
    /* Restore mstatus */
    lw      t0,   112(sp)
    csrw    mstatus, t0
    /* Restore registers */
    lw      s11,  108(sp)
    lw      s10,  104(sp)
    lw      s9,   100(sp)
    lw      s8,   96(sp)
    lw      s7,   92(sp)
    lw      s6,   88(sp)
    lw      s5,   84(sp)
    lw      s4,   80(sp)
    lw      s3,   76(sp)
    lw      s2,   72(sp)
    lw      s1,   68(sp)
    lw      s0,   64(sp)
    lw      a7,   60(sp)
    lw      a6,   56(sp)
    lw      a5,   52(sp)
    lw      a4,   48(sp)
    lw      a3,   44(sp)
    lw      a2,   40(sp)
    lw      a1,   36(sp)
    /* a0 (next TCB pointer) is in a0, skip slot */
    lw      t6,   28(sp)
    lw      t5,   24(sp)
    lw      t4,   20(sp)
    lw      t3,   16(sp)
    lw      t2,   12(sp)
    lw      t1,   8(sp)
    lw      t0,   4(sp)
    lw      ra,   0(sp)
    addi    sp,   sp,  4*34       /* deallocate stack frame */
    ret
    /* Save mstatus CSR */
    csrr    t0, mstatus
    sw      t0,  4*28(sp)
    /* Restore mstatus CSR */
    lw      t0,  4*28(sp)
    csrw    mstatus, t0
    /* Restore mstatus CSR */
    lw      t0,   112(sp)
    csrw    mstatus, t0

    /* Save mstatus CSR */
    csrr    t0,   mstatus
    sw      t0,   112(sp)
    .global portRESTORE_CONTEXT
portRESTORE_CONTEXT:
    # a0 = pointer to current TCB''s stack pointer storage

    # 1) Load SP from TCB
    lw      sp,   116(a0)

    # 2) Restore mstatus
    lw      t0,   112(sp)
    csrw    mstatus, t0

    # 3) Restore saved registers in reverse order
    lw      s11, 108(sp)
    lw      s10, 104(sp)
    lw      s9,  100(sp)
    lw      s8,   96(sp)
    lw      s7,   92(sp)
    lw      s6,   88(sp)
    lw      s5,   84(sp)
    lw      s4,   80(sp)
    lw      s3,   76(sp)
    lw      s2,   72(sp)
    lw      s1,   68(sp)
    lw      s0,   64(sp)
    lw      a7,   60(sp)
    lw      a6,   56(sp)
    lw      a5,   52(sp)
    lw      a4,   48(sp)
    lw      a3,   44(sp)
    lw      a2,   40(sp)
    lw      a1,   36(sp)
    lw      a0,   32(sp)
    lw      t6,   28(sp)
    lw      t5,   24(sp)
    lw      t4,   20(sp)
    lw      t3,   16(sp)
    lw      t2,   12(sp)
    lw      t1,    8(sp)
    lw      t0,    4(sp)
    lw      ra,    0(sp)

    # 4) Adjust SP back to before save
    addi    sp, sp, 4*34

    # 5) Return to the task''s saved PC
    mret
    .global portRESTORE_CONTEXT
portRESTORE_CONTEXT:
    # a0 = pointer to current TCB’s stack pointer storage

    # 1) Load SP from TCB
    lw      sp,   116(a0)

    # 2) Restore mstatus
    lw      t0,   112(sp)
    csrw    mstatus, t0

    # 3) Restore GPRs in reverse order
    lw      s11, 108(sp)
    lw      s10, 104(sp)
    lw      s9,  100(sp)
    lw      s8,   96(sp)
    lw      s7,   92(sp)
    lw      s6,   88(sp)
    lw      s5,   84(sp)
    lw      s4,   80(sp)
    lw      s3,   76(sp)
    lw      s2,   72(sp)
    lw      s1,   68(sp)
    lw      s0,   64(sp)
    lw      a7,   60(sp)
    lw      a6,   56(sp)
    lw      a5,   52(sp)
    lw      a4,   48(sp)
    lw      a3,   44(sp)
    lw      a2,   40(sp)
    lw      a1,   36(sp)
    lw      a0,   32(sp)
    lw      t6,   28(sp)
    lw      t5,   24(sp)
    lw      t4,   20(sp)
    lw      t3,   16(sp)
    lw      t2,   12(sp)
    lw      t1,    8(sp)
    lw      t0,    4(sp)
    lw      ra,    0(sp)

    # 4) Restore SP
    addi    sp, sp, 4*34

    # 5) Return to task via mret
    mret
