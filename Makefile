# Makefile

VIVADO         := vivado
RISCV_GCC      := riscv32-unknown-elf-gcc
RISCV_OBJCOPY  := riscv32-unknown-elf-objcopy
OPENFPGALOADER := openFPGALoader
OPENOCD        := openocd

SRC_VERILOG  := src/verilog
SRC_C        := src/c
INCLUDE      := include
PORTABLE     := freertos/portable/RISC-V
BUILD        := build

all: bit firmware

bit:
	$(VIVADO) -mode batch -source scripts/build_fpga.tcl \
	  -tclargs $(SRC_VERILOG) build/top.bit

firmware:
	$(RISCV_GCC) -march=rv32imac -mabi=ilp32 -O2 \
	  -I$(INCLUDE) -I$(PORTABLE) \
	  $(PORTABLE)/*.c $(SRC_C)/*.c \
	  -T linker.ld -o $(BUILD)/main.elf
	$(RISCV_OBJCOPY) -O binary $(BUILD)/main.elf $(BUILD)/main.bin

flash: bit firmware
	$(OPENFPGALOADER) --fpga-model "Nexys A7" build/top.bit
	$(OPENOCD) -f interface/ftdi/board.cfg -c "program build/main.bin 0x20000000 reset exit"

clean:
	rm -rf $(BUILD)/*

.PHONY: all bit firmware flash clean
