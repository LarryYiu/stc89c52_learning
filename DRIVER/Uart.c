#include "Uart.h"

char putchar (char c)  
{
  SBUF = c;      
  while (!TI);
  TI = 0;
  return c;
}

void UART_Init(void)  // 9600bps@11.0592MHz
{
    SCON = 0x50;  // 8 bits and variable baudrate
    T2CON = 0x34;
    RCAP2L = TL2 = 0xDC;  // Initial timer value
    RCAP2H = TH2 = 0xFF;  // Initial timer value
    TR2 = 1;              // Timer2 start run
    EA = 1;
    ES = 1;
}

void UART_SendByte(uint8_t* datPtr, bool* busyFlagPtr)
{
    while (*busyFlagPtr);
    *busyFlagPtr = true;
    SBUF = *datPtr;
}

void UART_SendInt(uint16_t* datPtr, bool* busyFlagPtr)
{
    uint8_t highByte = (*datPtr) >> 8;
    uint8_t lowByte = (*datPtr) & 0xFF;
    UART_SendByte(&highByte, busyFlagPtr);
    UART_SendByte(&lowByte, busyFlagPtr);
}

void UART_SendString(uint8_t* s, bool* busyFlagPtr)
{
    while (*s)  // Check the end of the string
    {
        UART_SendByte(
            s++, busyFlagPtr);  // Send current char and increment string ptr
    }
}
