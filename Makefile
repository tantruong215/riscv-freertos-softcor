# Makefile

VIVADO         := vivado
RISCV_GCC      := riscv32-unknown-elf-gcc
RISCV_OBJCOPY  := riscv32-unknown-elf-objcopy
OPENFPGALOADER := openFPGALoader
OPENOCD        := openocd

TB             := tb
SRC_VERILOG    := src/verilog
SRC_C          := src/c
INCLUDE        := include
PORTABLE       := freertos/portable/RISC-V
BUILD          := build

all: bit firmware

bit:
	$(VIVADO) -mode batch -source scripts/build_fpga.tcl \
	  -tclargs $(SRC_VERILOG) $(BUILD)/top.bit

firmware:
	@mkdir -p $(BUILD) $(TB)
	$(RISCV_GCC) -march=rv32imac -mabi=ilp32 -O2 \
	  -I$(INCLUDE) -I$(PORTABLE) \
	  $(PORTABLE)/*.c $(SRC_C)/*.c \
	  -T linker.ld -o $(BUILD)/main.elf
	$(RISCV_OBJCOPY) -O binary $(BUILD)/main.elf $(BUILD)/main.bin
	$(RISCV_OBJCOPY) -O verilog $(BUILD)/main.elf $(TB)/firmware.mem

flash: bit firmware
	$(OPENFPGALOADER) --fpga-model "Nexys A7" $(BUILD)/top.bit
	$(OPENOCD) -f interface/ftdi/board.cfg -c "program $(BUILD)/main.bin 0x20000000 reset exit"

clean:
	rm -rf $(BUILD)/* $(TB)/firmware.mem

.PHONY: all bit firmware flash clean
