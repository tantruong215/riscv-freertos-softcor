#define RX_BUF_SIZE  128
#define TX_BUF_SIZE  128
typedef struct {
    uint8_t buffer[RX_BUF_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} RingBuffer;
extern RingBuffer uart_rx_buf, uart_tx_buf;
