// src/verilog/uart.v
module uart #(
    parameter CLOCK_FREQ = 50_000_000,  // System clock: 50 MHz
    parameter BAUD_RATE  = 115200       // UART baud rate
) (
    input  wire        clk,
    input  wire        rst,
    input  wire        rx,
    output reg         tx,
    output reg         rx_valid,
    output reg  [7:0]  rx_data,
    input  wire [7:0]  tx_data,
    input  wire        tx_start,
    output reg         tx_busy
);
  //–– Baud generator ––//
  localparam integer BAUD_DIV = CLOCK_FREQ / BAUD_RATE;
  reg [31:0]        baud_cnt;
  wire              baud_tick = (baud_cnt == 0);
  always @(posedge clk) begin
    if (baud_cnt == 0)
      baud_cnt <= BAUD_DIV;
    else
      baud_cnt <= baud_cnt - 1;
  end

  //–– TX state machine ––//
  reg [3:0]   tx_state;
  reg [7:0]   tx_shift;
  always @(posedge clk or posedge rst) begin
    if (rst) begin
      tx_state <= 0;
      tx_busy  <= 0;
      tx       <= 1;
    end else if (tx_start && !tx_busy) begin
      tx_shift <= tx_data;
      tx_state <= 0;
      tx_busy  <= 1;
    end else if (baud_tick && tx_busy) begin
      case (tx_state)
        4'd0:  tx <= 0;
        4'd1:  tx <= tx_shift[0];
        4'd2:  tx <= tx_shift[1];
        4'd3:  tx <= tx_shift[2];
        4'd4:  tx <= tx_shift[3];
        4'd5:  tx <= tx_shift[4];
        4'd6:  tx <= tx_shift[5];
        4'd7:  tx <= tx_shift[6];
        4'd8:  tx <= tx_shift[7];
        4'd9:  tx <= 1;
        4'd10: begin tx_busy <= 0; tx <= 1; end
      endcase
      tx_state <= tx_state + 1;
    end
  end

  //–– RX state machine ––//
  reg [3:0]   rx_state;
  reg [7:0]   rx_shift;
  reg [31:0]  rx_cnt;
  always @(posedge clk or posedge rst) begin
    if (rst) begin
      rx_state <= 0;
      rx_valid <= 0;
      rx_data  <= 0;
      rx_cnt   <= 0;
    end else if (baud_tick) begin
      case (rx_state)
        4'd0: if (!rx) begin
                 rx_state <= 4'd1;
                 rx_cnt   <= BAUD_DIV/2;
               end
        4'd1: if (rx_cnt==0) begin
                 rx_state <= 4'd2;
                 rx_cnt   <= BAUD_DIV;
               end else rx_cnt <= rx_cnt-1;
        4'd2,4'd3,4'd4,4'd5,4'd6,4'd7,4'd8,4'd9: begin
               if (rx_cnt==0) begin
                 rx_shift[rx_state-2] <= rx;
                 rx_state <= rx_state+1;
                 rx_cnt   <= BAUD_DIV;
               end else rx_cnt <= rx_cnt-1;
             end
        4'd10: begin
               rx_valid <= 1;
               rx_data  <= rx_shift;
               rx_state <= 0;
             end
      endcase
    end else
      rx_valid <= 0;
  end

endmodule
