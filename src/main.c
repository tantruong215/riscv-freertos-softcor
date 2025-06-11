
// Trap vector init
extern void _vector_table(void);
static void init_traps(void)
{
    /* mtvec = &_vector_table | vectored mode */
    uintptr_t vt = (uintptr_t)&_vector_table | 1;
    __asm volatile("csrw mtvec, %0" :: "r"(vt));

    /* Enable M-timer interrupt in MIE */
    __asm volatile("csrrs zero, mie, %0" :: "r"(1 << 7));

    /* Globally enable interrupts (MIE bit in mstatus) */
    __asm volatile("csrsi mstatus, %0" :: "i"(1 << 3));
}

int main(void)
{
    init_traps();
    // … your existing setup …
    vTaskStartScheduler();
    for (;;);
}

// Trap vector init
extern void _vector_table(void);
static void init_traps(void)
{
    /* mtvec = &_vector_table | vectored mode */
    uintptr_t vt = (uintptr_t)&_vector_table | 1;
    __asm volatile("csrw mtvec, %0" :: "r"(vt));

    /* Enable M-timer interrupt in MIE */
    __asm volatile("csrrs zero, mie, %0" :: "r"(1 << 7));

    /* Globally enable interrupts (MIE bit in mstatus) */
    __asm volatile("csrsi mstatus, %0" :: "i"(1 << 3));
}

int main(void)
{
    init_traps();
    // … your existing setup …
    vTaskStartScheduler();
    for (;;);
}
