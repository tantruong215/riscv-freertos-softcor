// Simple, parameterized UART RX/TX with 1-byte FIFO behavior.

module uart #(
    parameter CLOCK_FREQ = 50_000_000,  // 50 MHz system clock
    parameter BAUD_RATE  = 115200       // 115 200 baud
) (
    input  wire        clk,
    input  wire        rst,

    // RX interface
    input  wire        rx,
    output reg         rx_valid,
    output reg  [7:0]  rx_data,

    // TX interface
    input  wire [7:0]  tx_data,
    input  wire        tx_start,
    output reg         tx,
    output reg         tx_busy
);

  // calculate divider
  localparam integer BAUD_DIV = CLOCK_FREQ / BAUD_RATE;

  //–– Baud‐rate generator ––//
  reg [15:0] baud_cnt;
  reg        baud_tick;
  always @(posedge clk) begin
    if (rst) begin
      baud_cnt  <= 0;
      baud_tick <= 0;
    end else if (baud_cnt == BAUD_DIV-1) begin
      baud_cnt  <= 0;
      baud_tick <= 1;
    end else begin
      baud_cnt  <= baud_cnt + 1;
      baud_tick <= 0;
    end
  end

  //–– RX state machine ––//
  reg [3:0]   rx_state;
  reg [3:0]   rx_bitcnt;
  reg [7:0]   rx_shift;
  reg [15:0]  rx_divcnt;
  wire        rx_start = (rx_state == 0) && !rx;  // start bit detected

  always @(posedge clk) begin
    if (rst) begin
      rx_state   <= 0;
      rx_valid   <= 0;
      rx_data    <= 0;
      rx_bitcnt  <= 0;
      rx_shift   <= 0;
      rx_divcnt  <= 0;
    end else begin
      rx_valid <= 0;
      case (rx_state)
        0: if (rx_start) begin
             rx_state  <= 1;
             rx_divcnt <= BAUD_DIV >> 1;  // sample in middle
           end
        1: if (baud_tick) begin
             if (rx_divcnt == 0) begin
               rx_bitcnt <= 0;
               rx_state  <= 2;
             end else
               rx_divcnt <= rx_divcnt - 1;
           end
        2: if (baud_tick) begin
             rx_shift <= {rx, rx_shift[7:1]};
             rx_bitcnt <= rx_bitcnt + 1;
             if (rx_bitcnt == 7)
               rx_state <= 3;
           end
        3: if (baud_tick) begin
             rx_valid <= 1;
             rx_data  <= rx_shift;
             rx_state <= 0;
           end
      endcase
    end
  end

  //–– TX state machine ––//
  reg [3:0]  tx_state;
  reg [3:0]  tx_bitcnt;
  reg [7:0]  tx_shift;
  reg [15:0] tx_divcnt;

  always @(posedge clk) begin
    if (rst) begin
      tx_state  <= 0;
      tx        <= 1;  // idle high
      tx_busy   <= 0;
      tx_bitcnt <= 0;
      tx_shift  <= 0;
      tx_divcnt <= 0;
    end else begin
      case (tx_state)
        0: if (tx_start) begin
             tx_busy   <= 1;
             tx_shift  <= tx_data;
             tx_bitcnt <= 0;
             tx       <= 0;  // start bit
             tx_divcnt <= BAUD_DIV-1;
             tx_state <= 1;
           end
        1: if (baud_tick) begin
             if (tx_divcnt == 0) begin
               tx_divcnt <= BAUD_DIV-1;
               tx_state  <= 2;
               tx        <= tx_shift[0];
               tx_shift  <= tx_shift >> 1;
             end else
               tx_divcnt <= tx_divcnt - 1;
           end
        2: if (baud_tick) begin
             if (tx_divcnt == 0) begin
               if (tx_bitcnt == 7) begin
                 tx_state <= 3;
                 tx       <= 1;  // stop bit
               end else begin
                 tx_bitcnt <= tx_bitcnt + 1;
                 tx        <= tx_shift[0];
                 tx_shift  <= tx_shift >> 1;
               end
               tx_divcnt <= BAUD_DIV-1;
             end else
               tx_divcnt <= tx_divcnt - 1;
           end
        3: if (baud_tick) begin
             tx_busy  <= 0;
             tx_state <= 0;
           end
      endcase
    end
  end

endmodule
