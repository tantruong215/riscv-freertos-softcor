# scripts/build_fpga.tcl
set verilog_dir [lindex $argv 0]
set out_bit     [lindex $argv 1]

read_verilog $verilog_dir/top.v
read_verilog $verilog_dir/uart.v
read_verilog vendor/picorv32/picorv32.v

set_part xc7a100tcsg324-1
create_clock -period 20.0 [get_ports clk]  ;# 50 MHz

synth_design -top top -part xc7a100tcsg324-1
opt_design
place_design
route_design
write_bitstream $out_bit
exit
