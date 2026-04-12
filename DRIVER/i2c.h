#ifndef __I2C_H__
#define __I2C_H__

#include <STC89C5xRC.H>
#include <intrins.h>

#include "Utility.h"

#define __SCL_PIN__ P20
#define __SDA_PIN__ P21

// sbit __SCL_PIN__ = P2 ^ 0;
// sbit __SDA_PIN__ = P2 ^ 1;
/**
 * @brief I2C start condition: SDA goes from HIGH to LOW while SCL is HIGH
 */
void I2C_Start();

/**
 * @brief I2C stop condition: SDA goes from LOW to HIGH while SCL is HIGH
 */
void I2C_Stop();

/**
 * @brief Send a byte of data over I2C.
 * @param dat The byte of data to send.
 */
void I2C_SendByte(uint8_t dat);

/**
 * @brief Receive a byte of data over I2C.
 * @return The byte of data received.
 */
uint8_t I2C_ReceiveByte();

/**
 * @brief Send an ACK or NACK bit over I2C.
 * @param ackBit If true, send an ACK (0); if false, send a NACK (1).
 */
void I2C_SendAck(uint8_t ackBit);

/**
 * @brief Receive an ACK or NACK bit over I2C.
 * @return 0 if an ACK was received, 1 if a NACK was received.
 */
uint8_t I2C_ReceiveAck();

#endif  //__I2C_H__