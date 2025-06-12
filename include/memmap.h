#ifndef MEMMAP_H
#define MEMMAP_H

/* Core Local Interruptor (CLINT) base for machine timer */
#define CLINT_BASE        0x02000000UL
#define MTIME_OFFSET      0xBFF8UL
#define MTIMECMP_OFFSET   0x4000UL

/* Access macros for machine timer registers */
#define MTIME    (*(volatile uint64_t *)(CLINT_BASE + MTIME_OFFSET))
#define MTIMECMP (*(volatile uint64_t *)(CLINT_BASE + MTIMECMP_OFFSET))

#endif /* MEMMAP_H */
