#define SPI_BUF_SIZE 128
typedef struct {
    uint8_t buffer[SPI_BUF_SIZE];
    volatile uint16_t head, tail;
} RingBuffer;
extern RingBuffer spi_tx_buf, spi_rx_buf;
typedef struct {
    uint8_t      buffer[SPI_BUF_SIZE];
    volatile uint16_t head, tail;
    volatile bool    overflow;
} RingBuffer;
bool RingBuffer_HasOverflow(RingBuffer *rb);
void RingBuffer_ClearOverflow(RingBuffer *rb);
extern RingBuffer spi_rx_buf, spi_tx_buf;
