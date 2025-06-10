`timescale 1ns/1ps

module uart_tb;

  // Parameters must match your uart.v
  parameter CLK_FREQ  = 50_000_000;
  parameter BAUD_RATE = 115200;
  localparam BIT_TIME = CLK_FREQ / BAUD_RATE;

  // DUT signals
  reg         clk = 0;
  reg         rst = 1;
  reg         rx  = 1;       // idle high
  wire        tx;
  wire        rx_valid;
  wire [7:0]  rx_data;
  reg  [7:0]  tx_data;
  reg         tx_start;
  wire        tx_busy;

  // Clock generation
  always #10 clk = !clk;      // 50 MHz

  // Instantiate your UART
  uart #(
    .CLOCK_FREQ(CLK_FREQ),
    .BAUD_RATE (BAUD_RATE)
  ) dut (
    .clk      (clk),
    .rst      (rst),
    .rx       (rx),
    .tx       (tx),
    .rx_valid (rx_valid),
    .rx_data  (rx_data),
    .tx_data  (tx_data),
    .tx_start (tx_start),
    .tx_busy  (tx_busy)
  );

  initial begin
    // Release reset
    #100;
    rst = 0;
    
    // Drive an RX frame: send 0x5A (?01011010?) LSB first
    send_byte(8'h5A);

    // Wait, then start a TX via tx_start
    #1000;
    tx_data  = 8'hA5;   // echo pattern
    tx_start = 1;
    #20;
    tx_start = 0;

    // Let it finish
    #(BIT_TIME*12);

    $display("RX valid: %b, data = %02X", rx_valid, rx_data);
    $finish;
  end

  // Task to bit-bang RX
  task send_byte(input [7:0] b);
    integer i;
    begin
      // start bit
      rx = 0;
      #(BIT_TIME);
      // data bits
      for (i = 0; i < 8; i = i + 1) begin
        rx = b[i];
        #(BIT_TIME);
      end
      // stop bit
      rx = 1;
      #(BIT_TIME);
    end
  endtask

  // Monitor TX and reconstruct transmitted byte
  reg [7:0] rx_shift;
  integer   bit_cnt;
  initial begin
    @(negedge tx);           // detect start bit
    # (BIT_TIME + BIT_TIME/2);
    for (bit_cnt = 0; bit_cnt < 8; bit_cnt = bit_cnt + 1) begin
      rx_shift[bit_cnt] = tx;
      #BIT_TIME;
    end
    #BIT_TIME;
    $display("Transmitted byte: %02X", rx_shift);
  end

endmodule
