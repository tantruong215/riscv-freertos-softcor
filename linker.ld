/* linker.ld */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512K
  SRAM  (rwx): ORIGIN = 0x20000000, LENGTH = 4K
}
SECTIONS
{
  .text   : { *(.text*) }  > FLASH
  .rodata : { *(.rodata*) } > FLASH
  .data   : { *(.data*) }  > SRAM
  .bss    : { *(.bss*) }   > SRAM
}
