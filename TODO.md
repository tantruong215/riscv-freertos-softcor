# TODO – RISC-V + FreeRTOS Soft Core

- [ ] Review PicoRV32 RTL and FreeRTOS compatibility
- [ ] Write Verilog UART + SPI peripheral drivers
- [ ] Validate UART loopback using testbench
- [ ] Port basic FreeRTOS kernel (heap config, tasks)
- [ ] Blink LED task and log SPI samples to UART
- [ ] Document integration process and core constraints

JUNE 9th 
# TODO

- [ ] Complete FreeRTOS port layer  
  - Implement `portSAVE_CONTEXT` and `portRESTORE_CONTEXT` routines  
  - Configure and initialize machine-timer (SysTick equivalent)  
  - Set up interrupt vector to call FreeRTOS tick handler  
- [ ] Develop and verify peripheral drivers  
  - SPI master driver with loopback testbench  
  - UART driver with transmit/receive tests  
  - I²C driver with simple read/write verification  
- [ ] Create end-to-end FreeRTOS demo  
  - Write a multi-task application (LED blink, sensor read, UART log)  
  - Automate simulation script to boot the RTOS and check console output  
- [ ] Add CI integration  
  - GitHub Actions workflow to run RTL simulations and firmware builds on push  
  - Badge in README showing build & test status  
- [ ] Enhance documentation  
  - Flesh out “Getting Started” steps in README (tool versions, clone → build → simulate)  
  - Add memory-map diagram for instruction RAM, data RAM, and peripherals  
