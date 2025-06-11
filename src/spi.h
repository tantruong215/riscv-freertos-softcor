#define SPI_BUF_SIZE 128
typedef struct {
    uint8_t buffer[SPI_BUF_SIZE];
    volatile uint16_t head, tail;
} RingBuffer;
extern RingBuffer spi_tx_buf, spi_rx_buf;
