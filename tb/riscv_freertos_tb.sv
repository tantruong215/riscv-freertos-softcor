`timescale 1ns/1ps

module riscv_freertos_tb;

  // Clock and reset
  reg clk = 0;
  reg rst_n = 0;
  always #10 clk = ~clk; // 50 MHz

  // SPI signals
  reg spi_sclk = 0;
  reg spi_mosi = 0;
  wire spi_miso;
  reg spi_cs_n = 1;

  // UART signals
  reg uart_rx = 1;
  wire uart_tx;

  // Machine-timer registers (via DPI or direct regs)
  // Model mtime and mtimecmp as regs
  reg [63:0] mtime = 0;
  reg [63:0] mtimecmp = 64'hFFFF_FFFF_FFFF_FFFF;

  // DUT instantiation
  top dut (
    .clk(clk),
    .rst_n(rst_n),
    // SPI
    .spi_sclk(spi_sclk),
    .spi_mosi(spi_mosi),
    .spi_miso(spi_miso),
    .spi_cs_n(spi_cs_n),
    // UART
    .uart_rx(uart_rx),
    .uart_tx(uart_tx),
    // Timer
    .mtime(mtime),
    .mtimecmp(mtimecmp)
  );

  // Monitor variables
  time tick_time;
  time irq_trigger_time;
  time context_switch_time;

  // Generate reset
  initial begin
    rst_n = 0;
    #100;
    rst_n = 1;
  end

  // Simulate machine-timer increment
  always @(posedge clk) begin
    if (rst_n)
      mtime <= mtime + 1;
  end

  // Fire mtimer interrupt every 1000 cycles (1 kHz)
  initial begin
    @(posedge rst_n);
    forever begin
      // schedule next compare
      @(posedge clk);
      mtimecmp <= mtime + 1000;
      irq_trigger_time = $time;
      // wait for interrupt context switch to complete
      wait(dut.ported && dut.switched); // replace with actual flags
      context_switch_time = $time - irq_trigger_time;
      $display("Context-switch latency: %0t ns", context_switch_time);
      #1000000;
    end
  end

  // SPI stimulus
  initial begin
    @(posedge rst_n);
    #500;
    forever begin
      spi_cs_n = 0;
      repeat (8) begin
        spi_sclk = 0; #10;
        spi_mosi = $random;
        spi_sclk = 1; #10;
      end
      spi_cs_n = 1;
      #10000;
    end
  end

  // UART RX stimulus: echo 'A' every 20000 ns
  task send_uart(input [7:0] data);
    integer i;
    integer bit_time = 50000000/115200;
    begin
      // start bit
      uart_rx = 0;
      #(bit_time);
      // data bits
      for (i=0; i<8; i=i+1) begin
        uart_rx = data[i];
        #(bit_time);
      end
      // stop bit
      uart_rx = 1;
      #(bit_time);
    end
  endtask

  initial begin
    @(posedge rst_n);
    #10000;
    forever begin
      send_uart(8'h41);
      #20000;
    end
  end

  // End simulation
  initial begin
    #200_000_000;
    $finish;
  end

endmodule
