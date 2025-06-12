`timescale 1ns/1ps
`include "picorv32.v"
`include "uart.v"

module top(
  input  wire       clk,
  input  wire       rst,
  input  wire       uart_rx,
  output wire       uart_tx,
  output wire [7:0] led
);

  //–– PicoRV32 core wires ––//
  wire        mem_valid;
  wire [31:0] mem_addr;
  wire [31:0] mem_wdata;
  wire [3:0]  mem_wstrb;
  wire        mem_instr;
  wire        mem_ready;
  wire [31:0] mem_rdata;

  //–– Connect core to simple synchronous RAM ––//
  picorv32 #(.ENABLE_MUL(1), .ENABLE_DIV(1)) cpu (
    .clk       (clk),
    .resetn    (!rst),
    .mem_valid (mem_valid),
    .mem_addr  (mem_addr),
    .mem_wdata (mem_wdata),
    .mem_wstrb (mem_wstrb),
    .mem_instr (mem_instr),
    .mem_rdata (mem_rdata),
    .mem_ready (mem_ready)
  );

  //–– 64 KB RAM (byte-addressable) ––//
  reg [31:0] ram [0:16383];
  reg [31:0] rdata_reg;
  always @(posedge clk) begin
    if (mem_valid && |mem_wstrb) begin
      if (mem_wstrb[0]) ram[mem_addr[15:2]][ 7: 0] <= mem_wdata[ 7: 0];
      if (mem_wstrb[1]) ram[mem_addr[15:2]][15: 8] <= mem_wdata[15: 8];
      if (mem_wstrb[2]) ram[mem_addr[15:2]][23:16] <= mem_wdata[23:16];
      if (mem_wstrb[3]) ram[mem_addr[15:2]][31:24] <= mem_wdata[31:24];
      rdata_reg <= mem_wdata;  // write-through for reads
    end else begin
      rdata_reg <= ram[mem_addr[15:2]];
    end
  end
  assign mem_rdata = rdata_reg;
  assign mem_ready = 1;

  //–– UART echo peripheral ––//
  wire [7:0] uart_rx_byte;
  wire       uart_rx_valid;
  reg        uart_tx_start;
  wire       uart_tx_busy;

  uart #(
    .CLOCK_FREQ(50_000_000),
    .BAUD_RATE (115200)
  ) u_uart (
    .clk       (clk),
    .rst       (rst),
    .rx        (uart_rx),
    .rx_valid  (uart_rx_valid),
    .rx_data   (uart_rx_byte),
    .tx_data   (uart_rx_byte),
    .tx_start  (uart_rx_valid),
    .tx        (uart_tx),
    .tx_busy   (uart_tx_busy)
  );

  //–– LED heartbeat ––//
  reg [23:0] cnt;
  always @(posedge clk) begin
    if (rst) cnt <= 0;
    else      cnt <= cnt + 1;
  end
  assign led = cnt[23:16];

endmodule
