#include "i2c.h"
#include "ringbuffer.h"
#include "memmap.h"      /* I define I2C_BASE_ADDR in here */
#include <stdbool.h>

/* These offsets match the I²C block I laid out in my FPGA design */
#define I2C_BASE           0x10020000U
#define I2C_IRQ_ENABLE     (*(volatile uint32_t *)(I2C_BASE + 0x00))
#define I2C_IRQ_STATUS     (*(volatile uint32_t *)(I2C_BASE + 0x04))
#define I2C_DATA           (*(volatile uint8_t  *)(I2C_BASE + 0x08))
#define I2C_CTRL           (*(volatile uint32_t *)(I2C_BASE + 0x0C))

#define I2C_IRQ_RX_READY   (1U << 0)
#define I2C_IRQ_TX_READY   (1U << 1)
#define I2C_CTRL_TX_ENABLE (1U << 0)

/* My transmit and receive buffers */
static RingBuffer i2c_tx_buf;
static RingBuffer i2c_rx_buf;

void I2C_Init(void)
{
    /* I set up my ring buffers */
    RingBuffer_Init(&i2c_tx_buf, I2C_BUF_SIZE);
    RingBuffer_Init(&i2c_rx_buf, I2C_BUF_SIZE);

    /* I enable both RX and TX interrupts in the hardware */
    I2C_IRQ_ENABLE = I2C_IRQ_RX_READY | I2C_IRQ_TX_READY;
}

bool I2C_SendByte(uint8_t byte)
{
    if (RingBuffer_IsFull(&i2c_tx_buf)) {
        /* My buffer is full—caller will know to retry or drop */
        return false;
    }
    RingBuffer_Put(&i2c_tx_buf, byte);
    /* I kick off the hardware if it was idle */
    I2C_CTRL |= I2C_CTRL_TX_ENABLE;
    return true;
}

int I2C_ReceiveByte(void)
{
    if (RingBuffer_IsEmpty(&i2c_rx_buf)) {
        /* No data yet */
        return -1;
    }
    /* I return the next byte in my buffer */
    return (int)RingBuffer_Get(&i2c_rx_buf);
}

void I2C_IRQHandler(void)
{
    uint32_t status = I2C_IRQ_STATUS;

    /* If RX is ready, I read one byte and store it */
    if (status & I2C_IRQ_RX_READY) {
        uint8_t b = I2C_DATA;
        if (!RingBuffer_Put(&i2c_rx_buf, b)) {
            /* I flag overflow if it happens */
            RingBuffer_SetOverflow(&i2c_rx_buf, true);
        }
    }

    /* If TX is ready, I send the next byte or disable TX */
    if (status & I2C_IRQ_TX_READY) {
        if (!RingBuffer_IsEmpty(&i2c_tx_buf)) {
            I2C_DATA = RingBuffer_Get(&i2c_tx_buf);
        } else {
            /* I turn off TX engine when no more data */
            I2C_CTRL &= ~I2C_CTRL_TX_ENABLE;
        }
    }

    /* I clear the bits I serviced */
    I2C_IRQ_STATUS = status;
}
