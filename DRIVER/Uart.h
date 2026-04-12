#ifndef __UART_H__
#define __UART_H__

#include <STC89C5xRC.H>
#include <stdio.h>

#include "Utility.h"

/**
 * @brief Initialize the UART related registers for serial communication
 */
void UART_Init();

/**
 * @brief Send a byte of data via UART
 *
 * @param datPtr Pointer to the data byte to send
 * @param busyFlagPtr Pointer to a flag indicating if the UART is busy
 */
void UART_SendByte(uint8_t* datPtr, bool* busyFlagPtr);

/**
 * @brief Send a 16-bit integer via UART
 *
 * @param datPtr Pointer to the 16-bit integer to send
 * @param busyFlagPtr Pointer to a flag indicating if the UART is busy
 *
 * @note The 16-bit integer is sent as two bytes, with the high byte sent first.
 */
void UART_SendInt(uint16_t* datPtr, bool* busyFlagPtr);

/**
 * @brief Send a null-terminated string via UART
 *
 * @param s Pointer to the null-terminated string to send
 * @param busyFlagPtr Pointer to a flag indicating if the UART is busy
 *
 * @note The function sends each character of the string one by one until it
 * reaches the null terminator. It uses the UART_SendByte function to send
 * each character.
 */
void UART_SendString(uint8_t* s, bool* busyFlagPtr);

#endif  // __UART_H__