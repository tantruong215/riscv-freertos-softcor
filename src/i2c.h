#define I2C_BUF_SIZE 128
typedef struct {
    uint8_t buffer[I2C_BUF_SIZE];
    volatile uint16_t head, tail;
} RingBuffer;
extern RingBuffer i2c_tx_buf, i2c_rx_buf;
