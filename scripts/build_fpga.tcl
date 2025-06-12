# scripts/build_fpga.tcl
# Usage: vivado -mode batch -source scripts/build_fpga.tcl -tclargs <out_bit> <constraint_xdc>

# Arguments
set out_bit [lindex $argv 0]
set xdc     [lindex $argv 1]

# Read your Verilog sources
read_verilog ../src/verilog/top.v
read_verilog ../src/verilog/uart.v
read_verilog ../src/verilog/picorv32.v

# Apply constraints
if {![file exists $xdc]} {
    puts "ERROR: Constraints file $xdc not found"
    exit 1
}
read_xdc $xdc

# FPGA part (update if different)
set_part xc7a100tcsg324-1

# Create the primary clock (50 MHz)
create_clock -period 20.0 [get_ports clk]

# Synthesize, implement, and generate bitstream
synth_design -top top
opt_design
place_design
route_design
write_bitstream $out_bit

exit
