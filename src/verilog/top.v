// src/verilog/top.v
`include "picorv32.v"

module top(
  input  wire        clk,
  input  wire        rst,
  input  wire        uart_rx,
  output wire        uart_tx,
  output wire [7:0]  led
);

  wire        rx_valid;
  wire [7:0]  rx_data;
  wire [7:0]  tx_data;
  wire        tx_start;
  wire        tx_busy;

  picorv32 #(
    .BARREL_SHIFTER(1),
    .COMPRESSED_ISA(1)
  ) cpu (
    .clk      (clk),
    .resetn   (!rst),
    .irq      (32'b0),
    .trap     (),
    .mem_valid(),
    .mem_instr(),
    .mem_addr (),
    .mem_wdata(),
    .mem_wstrb(),
    .mem_rdata()
  );

  uart #(
    .CLOCK_FREQ(50_000_000),
    .BAUD_RATE (115200)
  ) uart0 (
    .clk      (clk),
    .rst      (rst),
    .rx       (uart_rx),
    .tx       (uart_tx),
    .rx_valid (rx_valid),
    .rx_data  (rx_data),
    .tx_data  (tx_data),
    .tx_start (tx_start),
    .tx_busy  (tx_busy)
  );

  reg [7:0] echo_byte;
  reg       echo_req;
  always @(posedge clk) begin
    if (rx_valid && !tx_busy) begin
      echo_byte <= rx_data;
      echo_req  <= 1;
    end else begin
      echo_req  <= 0;
    end
  end

  assign tx_data  = echo_byte;
  assign tx_start = echo_req;

  reg [23:0] cnt;
  always @(posedge clk) cnt <= cnt + 1;
  assign led = cnt[23:16];

endmodule
