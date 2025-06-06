# RISC-V + FreeRTOS Soft Core on FPGA

Implements a soft-core RISC-V (PicoRV32) CPU running FreeRTOS on a Xilinx Artix-7 FPGA. Demonstrates real-time task scheduling and bare-metal peripheral control.

## System Architecture
- PicoRV32 @ 50 MHz
- 4 KB instruction RAM
- 2 KB data RAM
- UART, SPI, and I2C peripheral interfaces
- FreeRTOS v10.0.1 (preemptive multitasking)

## Software Features
- RTOS port for PicoRV32
- Three concurrent tasks:
  - SPI sensor sampling (1 kHz)
  - LED toggling (1 Hz)
  - UART data logging (115200 bps)

## Timing Results
- Interrupt latency: ~140 ns
- Task switch time: ~3 µs
- Sensor sampling jitter: <120 ns

## Tools Used
- Vivado + Verilog
- FreeRTOS
- Integrated Logic Analyzer (ILA)

## Project Status
In Progress – Summer 2025
