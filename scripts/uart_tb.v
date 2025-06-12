`timescale 1ns/1ps

module uart_tb;

  // Parameters
  parameter CLK_FREQ  = 50_000_000;
  parameter BAUD_RATE = 115200;
  localparam BIT_TIME = CLK_FREQ / BAUD_RATE;

  // Clock & reset
  reg clk = 0;
  reg rst = 1;

  // UART lines
  reg  rx = 1;              // idle high
  wire tx;
  wire rx_valid;
  wire [7:0] rx_data;

  // Instantiate your uart (echo back received byte)
  uart #(
    .CLOCK_FREQ(CLK_FREQ),
    .BAUD_RATE (BAUD_RATE)
  ) uut (
    .clk       (clk),
    .rst       (rst),
    .rx        (rx),
    .rx_valid  (rx_valid),
    .rx_data   (rx_data),
    .tx_data   (rx_data),
    .tx_start  (rx_valid),
    .tx        (tx),
    .tx_busy   ()
  );

  // 50 MHz clock: period = 20 ns
  always #10 clk = ~clk;

  initial begin
    #50 rst = 0;        // release reset
  end

  // Send one byte (0xA5) then finish
  initial begin
    @(negedge rst);
    #100;
    send_byte(8'hA5);
    # (BIT_TIME * 20);
    $finish;
  end

  // Task to drive serial IN on rx line
  task send_byte(input [7:0] b);
    integer i;
    begin
      rx = 0;            // start bit
      #(BIT_TIME);
      for (i = 0; i < 8; i = i + 1) begin
        rx = b[i];
        #(BIT_TIME);
      end
      rx = 1;            // stop bit
      #(BIT_TIME);
    end
  endtask

  // Print received data
  always @(posedge clk) begin
    if (rx_valid) begin
      $display("Time %0t: Received 0x%02h", $time, rx_data);
    end
  end

endmodule
