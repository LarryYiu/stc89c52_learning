#include <STC89C5xRC.H>
#include <stdio.h>

#include "DS18B20.h"
#include "Interrupt.h"
#include "Key.h"
#include "MatrixLed.h"
#include "Uart.h"
#include "Utility.h"
#include "config.h"

#define MATRIX_LED_DEBUG false

uint16_t xdata timer0counter = 0;

void Timer0() interrupt 1
{
    TH0 = TIMER0_H;
    TL0 = TIMER0_L;
    timer0counter++;
}

void Timer1() interrupt 3 {}

#if (MATRIX_LED_DEBUG)
void Uart() interrupt 4 {}
#endif  // MATRIX_LED_DEBUG

void main()
{
    uint8_t i;
    uint8_t keyNum = 0;
    uint8_t keyOffset = 0;
    uint16_t xdata matrixTik = 0;

    uint8_t speed = 2;
    // float xdata numTest = 31.5F;
    uint8_t xdata dat[48];
    uint8_t xdata datLen;

    int16_t xdata temperature = 0;
    uint16_t xdata temperatureTik = 0;
    // datLen = MatrixLed_ProcessFloat((float)temperature * 0.0625F, dat, true);
#if (MATRIX_LED_DEBUG)
    UART_Init();
#endif  // MATRIX_LED_DEBUG
    INTR_Init();
    while (1)
    {
        if (KEY_ScanStandalone_Busy(&keyNum))
        {
            if (keyNum == 1)
            {
                speed = speed > 3 ? 1 : speed + 1;
            }
            else if (keyNum == 2)
            {
                speed = speed < 1 ? 3 : speed - 1;
            }
#if (MATRIX_LED_DEBUG)
            printf("speed: %bu\n", speed);
#endif  // MATRIX_LED_DEBUG
        }
        for (i = 0; i < 8; i++)
        {
            MatrixLed_SendByte(matrixLedColLookup[i]);
            MatrixLed_SendByte(dat[i + keyOffset]);
            MatrixLed_Output();
        }
        if (timer0counter - matrixTik >= speed)
        {
            matrixTik = timer0counter;
            keyOffset = (keyOffset + 1) % (datLen * 8);
        }
        if (timer0counter - temperatureTik >= 20)
        {
            temperatureTik = timer0counter;
            temperature = DS18B20_ReadTemperature();
            datLen =
                MatrixLed_ProcessFloat((float)temperature * 0.0625F, dat, true);
        }
    }
}
