#ifndef __UART_H__
#define __UART_H__

#include <stdio.h>
#include <STC89C5xRC.H>

#include "Utility.h"

void UART_Init();

void UART_SendByte(uint8_t* datPtr, bool* busyFlagPtr);

void UART_SendInt(uint16_t* datPtr, bool* busyFlagPtr);

void UART_SendString(uint8_t* s, bool* busyFlagPtr);

#endif  // __UART_H__