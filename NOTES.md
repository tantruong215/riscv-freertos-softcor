# Development Notes – RISC-V + FreeRTOS

**June 6, 2025**  
Set up GitHub repo and initialized README. Planning architecture for FreeRTOS port on PicoRV32. Reviewing UART/SPI Verilog driver structure before task scheduling.

Next up:
- Start with UART echo loop
- Write Makefile for soft core + Vivado block design
- FreeRTOS port setup

**June 9, 2025**  
Spent time reviewing PicoRV32 architecture and FreeRTOS task model. Drafted system architecture — soft core runs at 50 MHz on Nexys A7, with 4 KB RAM for tasks. UART selected as debug channel; starting Verilog driver this week.

Goals:
- Implement UART echo loop as sanity check
- Validate task-switching via LED blink + UART combo
- Prepare memory map and interrupt handler for SPI ADC sampling
