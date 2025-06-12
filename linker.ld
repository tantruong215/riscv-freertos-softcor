/* Linker script for on-chip 64 KB RAM at address 0x0000 */

ENTRY(_start)

MEMORY
{
    RAM (rwx) : ORIGIN = 0x00000000, LENGTH = 64K
}

SECTIONS
{
    /* Place code (flash or RAM-backed) at address 0 */
    . = ORIGIN(RAM);

    /* Text (code) */
    .text :
    {
        KEEP(*(.init))
        *(.text .text.*)
        KEEP(*(.init_array .fini_array))
    } > RAM

    /* Read-only data */
    .rodata :
    {
        *(.rodata .rodata.*)
    } > RAM

    /* Initialized data */
    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        __data_load_start = LOADADDR(.data);
        __data_start      = .;
        *(.data .data.*)
        __data_end        = .;
    } > RAM

    /* Uninitialized data (zero-filled) */
    .bss :
    {
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end   = .;
    } > RAM

    /* Stacks, heap, and guard region go at the end of RAM */
    .heap (COPY):
    {
        __heap_start = .;
        . = ORIGIN(RAM) + LENGTH(RAM);
        __heap_end   = .;
    }

    /* Symbols for debugging */
    PROVIDE(_end   = __bss_end);
    PROVIDE(_stack = ORIGIN(RAM) + LENGTH(RAM));
}
