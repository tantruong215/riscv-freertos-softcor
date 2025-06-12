// tb/riscv_freertos_tb.sv
`timescale 1ns/1ps

module riscv_freertos_tb;

  // Clock & reset
  reg clk = 0;
  reg rst_n = 0;

  // UART lines
  wire uart_tx;
  reg  uart_rx = 1;

  // LED bus
  wire [7:0] led;

  // Memory model parameters
  localparam MEM_DEPTH = 16384;
  localparam ADDR_WIDTH = $clog2(MEM_DEPTH);

  //––––––––––––––– DUT –––––––––––––––//
  top uut (
    .clk      (clk),
    .rst      (~rst_n),
    .uart_rx  (uart_rx),
    .uart_tx  (uart_tx),
    .led      (led)
  );

  //––––––––––––––– Simple RAM –––––––––––––––//
  reg [31:0] ram [0:MEM_DEPTH-1];
  reg [31:0] rdata;

  // Load firmware into RAM at time 0
  initial begin
    $readmemh("../tb/firmware.mem", ram);
  end

  // Memory interface hooking (picoRV32 uses synchronous RAM)
  always_ff @(posedge clk) begin
    if (uut.mem_valid && |uut.mem_wstrb) begin
      // write bytes
      for (int i = 0; i < 4; i++) begin
        if (uut.mem_wstrb[i])
          ram[uut.mem_addr[ADDR_WIDTH+1:2]][8*i +: 8] <= uut.mem_wdata[8*i +: 8];
      end
      rdata <= uut.mem_wdata;  // write-through
    end else begin
      rdata <= ram[uut.mem_addr[ADDR_WIDTH+1:2]];
    end
  end

  assign uut.mem_rdata  = rdata;
  assign uut.mem_ready  = 1'b1;

  //––––––––––––––– Clock & Reset –––––––––––––––//
  always #10 clk = ~clk;  // 50 MHz

  initial begin
    // Hold reset low for 5 cycles
    #50 rst_n = 1;
  end

  //––––––––––––––– UART Monitor –––––––––––––––//
  integer uart_file;
  initial begin
    uart_file = $fopen("uart_output.log", "w");
    if (!uart_file) $error("Failed to open uart_output.log");
  end

  always @(posedge clk) begin
    if (uut.uart_tx_valid) begin
      // ascii byte on uart_tx_data
      $fwrite(uart_file, "%c", uut.uart_tx_data);
    end
  end

  //––––––––––––––– Simulation Stop –––––––––––––––//
  initial begin
    // run for 10 ms
    #10_000_000;
    $display("Simulation complete. UART output in uart_output.log");
    $finish;
  end

endmodule
