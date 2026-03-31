#include <STC89C5xRC.H>
#include <stdio.h>

#include "Interrupt.h"
#include "Uart.h"
#include "Utility.h"

void Uart() interrupt 4 {}

void Timer0() interrupt 1 {}

void main()
{
    INTR_Init();
    UART_Init();
    printf("INIT COMPLETED!\n");
    while (1)
    {
    }
    return;
}
