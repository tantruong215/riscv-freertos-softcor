#include "i2c.h"
#include <stdint.h>

/* Base address for your I²C peripheral */
#define I2C_BASE        0x10002000UL
#define I2C_CTRL        (*(volatile uint32_t *)(I2C_BASE + 0x00))
#define I2C_STATUS      (*(volatile uint32_t *)(I2C_BASE + 0x04))
#define I2C_ADDR        (*(volatile uint32_t *)(I2C_BASE + 0x08))
#define I2C_DATA        (*(volatile uint32_t *)(I2C_BASE + 0x0C))

/* Control/status bits (example) */
#define I2C_EN          (1 << 0)
#define I2C_START       (1 << 1)
#define I2C_STOP        (1 << 2)
#define I2C_ACK         (1 << 3)
#define I2C_TIP         (1 << 4)  /* transfer in progress */
#define I2C_RX_ACK      (1 << 5)

void i2c_init(void) {
    /* Enable I²C, set clock divider to CPUCLK/100 */
    I2C_CTRL = I2C_EN | (50 << 8);
}

int i2c_write(uint8_t dev_addr, uint8_t reg_addr, const uint8_t *data, size_t len) {
    /* Send START + write address */
    I2C_ADDR = (dev_addr << 1) | 0;
    I2C_CTRL |= I2C_START;
    while (I2C_STATUS & I2C_TIP);
    if (I2C_STATUS & I2C_RX_ACK) return -1;
    
    /* Send register address */
    I2C_DATA = reg_addr;
    while (I2C_STATUS & I2C_TIP);
    if (I2C_STATUS & I2C_RX_ACK) return -2;
    
    /* Send payload */
    for (size_t i = 0; i < len; i++) {
        I2C_DATA = data[i];
        while (I2C_STATUS & I2C_TIP);
        if (I2C_STATUS & I2C_RX_ACK) return -3;
    }
    
    /* STOP */
    I2C_CTRL |= I2C_STOP;
    while (I2C_STATUS & I2C_TIP);
    return 0;
}

int i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    /* Write reg_addr first (no STOP) */
    if (i2c_write(dev_addr, reg_addr, NULL, 0) < 0) return -1;
    
    /* Send repeated START + read address */
    I2C_ADDR = (dev_addr << 1) | 1;
    I2C_CTRL |= I2C_START;
    while (I2C_STATUS & I2C_TIP);
    if (I2C_STATUS & I2C_RX_ACK) return -2;
    
    /* Receive bytes */
    for (size_t i = 0; i < len; i++) {
        /* On last byte, send NACK+STOP */
        if (i == len - 1) {
            I2C_CTRL = I2C_EN | I2C_STOP;
        }
        while (I2C_STATUS & I2C_TIP);
        data[i] = I2C_DATA;
    }
    return 0;
}
// In i2c_init():
    i2c_tx_buf.head = i2c_tx_buf.tail = 0;
i2c_tx_buf.overflow = false;
i2c_rx_buf.head = i2c_rx_buf.tail = 0;
i2c_rx_buf.overflow = false;
    i2c_rx_buf.head = i2c_rx_buf.tail = 0;

// Add IRQ handler to src/i2c.c:
void I2C_IRQHandler(void) {
    uint32_t st = I2C_STATUS;
    if (st & I2C_RX_ACK == 0) {
        uint8_t b = I2C_DATA;
        uint16_t n = (i2c_rx_buf.head + 1) & (I2C_BUF_SIZE - 1);
        if (n != i2c_rx_buf.tail) {
            i2c_rx_buf.buffer[i2c_rx_buf.head] = b;
            i2c_rx_buf.head = n;
        }
    }
    if (!(st & I2C_TIP) && i2c_tx_buf.tail != i2c_tx_buf.head) {
        I2C_DATA = i2c_tx_buf.buffer[i2c_tx_buf.tail];
        i2c_tx_buf.tail = (i2c_tx_buf.tail + 1) & (I2C_BUF_SIZE - 1);
    }
}
// In I2C_IRQHandler:
    uint16_t ni = (i2c_rx_buf.head + 1) & (I2C_BUF_SIZE - 1);
    if (ni != i2c_rx_buf.tail) {
        i2c_rx_buf.buffer[i2c_rx_buf.head] = I2C_DATA;
        i2c_rx_buf.head = ni;
    } else {
        i2c_rx_buf.overflow = true;
    }
