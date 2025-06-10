# Development Notes – RISC-V + FreeRTOS

**June 6, 2025**  
Set up GitHub repo and initialized README. Planning architecture for FreeRTOS port on PicoRV32. Reviewing UART/SPI Verilog driver structure before task scheduling.

Goals:
Start with UART echo loop
Write Makefile for soft core + Vivado block design
FreeRTOS port setup

June 8, 2025
Spent time reviewing PicoRV32 architecture and FreeRTOS task model. Drafted system architecture — soft core runs at 50 MHz on Nexys A7, with 4 KB RAM for tasks. UART selected as debug channel; starting Verilog driver this week.

Goals:

Implement UART echo loop as sanity check
Validate task-switching via LED blink + UART combo
Prepare memory map and interrupt handler for SPI ADC sampling


June 10, 2025

Finished creating the freertos/portable/RISC-V directory structure.
Added initial portmacro.h definitions for RISC-V (stack frame layout, critical-section macros).
Stubbed out portSAVE_CONTEXT and portRESTORE_CONTEXT assembly functions.
Drafted machine-timer setup in port.c to generate FreeRTOS tick interrupts.
Reviewed and adjusted FreeRTOSConfig.h parameters (tick rate, heap model).

Goals:
Implement and unit-test context-save/restore routines in simulation
Finalize timer initialization and verify periodic tick in Icarus Verilog
Begin writing UART driver & echo loop sanity test
Draft memory map & interrupt handler outline for SPI-ADC task sampling
