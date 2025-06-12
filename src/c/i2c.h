#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>

/* This is the size I chose for my TX/RX queues */
#define I2C_BUF_SIZE  128

/* I’m using my ringbuffer implementation here */
#include "ringbuffer.h"

/* I initialize my I²C peripheral and enable its IRQs */
void I2C_Init(void);

/* My IRQ vector calls this when an I²C interrupt fires */
void I2C_IRQHandler(void);

/* I enqueue a byte to send; returns false if the buffer is full */
bool I2C_SendByte(uint8_t byte);

/* I dequeue a received byte, or return –1 if none is available */
int  I2C_ReceiveByte(void);

#endif /* I2C_H */
