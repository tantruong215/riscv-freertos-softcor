#include <stdint.h>

/* Point mtvec to your vectored table & enable Machine-Timer */
extern void _vector_table(void);
static void init_traps(void) {
    uintptr_t vt = (uintptr_t)&_vector_table | 1;    // vectored mode
    __asm volatile("csrw mtvec, %0" :: "r"(vt));
    __asm volatile("csrrs zero, mie, %0" :: "r"(1 << 7));
    __asm volatile("csrsi mstatus, %0" :: "i"(1 << 3));
}

int main(void) {
    init_traps();
    // Trap vector initint mainextern void _vector_table(void);int mainstatic void init_traps(void)int main{int main    /* mtvec = &_vector_table | vectored mode */int main    uintptr_t vt = (uintptr_t)&_vector_table | 1;int main    __asm volatile("csrw mtvec, %0" :: "r"(vt));int mainint main    /* Enable M-timer interrupt in MIE */int main    __asm volatile("csrrs zero, mie, %0" :: "r"(1 << 7));int mainint main    /* Globally enable interrupts (MIE bit in mstatus) */int main    __asm volatile("csrsi mstatus, %0" :: "i"(1 << 3));int main}int mainint mainint main(void)int main{int main    init_traps();int main    // ? your existing setup ?int main    vTaskStartScheduler();int main    for (;;);int main}int mainint main// Trap vector initint mainextern void _vector_table(void);int mainstatic void init_traps(void)int main{int main    /* mtvec = &_vector_table | vectored mode */int main    uintptr_t vt = (uintptr_t)&_vector_table | 1;int main    __asm volatile("csrw mtvec, %0" :: "r"(vt));int mainint main    /* Enable M-timer interrupt in MIE */int main    __asm volatile("csrrs zero, mie, %0" :: "r"(1 << 7));int mainint main    /* Globally enable interrupts (MIE bit in mstatus) */int main    __asm volatile("csrsi mstatus, %0" :: "i"(1 << 3));int main}int mainint mainint main(void)int main{int main    init_traps();int main    // ? your existing setup ?int main    vTaskStartScheduler();int main    for (;;);int main}
    // Create UART echo task
    xTaskCreate(vUARTEchoTask, "Echo", 128, NULL, tskIDLE_PRIORITY+1, NULL);
