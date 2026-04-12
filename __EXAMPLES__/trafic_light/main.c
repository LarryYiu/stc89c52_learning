#include <STC89C5xRC.H>
#include <stdio.h>

#include "I2C.h"
#include "Interrupt.h"
#include "Key.h"
#include "TM1650.h"
#include "TrafficLight.h"
#include "Utility.h"
#include "config.h"

uint8_t t0counter = 0;
uint8_t t1counter = 0;

/**
 * @brief Timer0: 50ms/interrupt, controls traffic light countdown
 */
void timer0() interrupt 1
{
    TH0 = TIMER0_H;
    TL0 = TIMER0_L;
    t0counter++;
    if (t0counter >= 20 / TL_SPEED_MULTIPLIER)
    {
        t0counter = 0;
        TL_AdvanceTimer();
        TL_HandleDT();
    }
}

/**
 * @brief Timer1: 25ms/interrupt, controls traffic light (LEDs) blinking
 */
void timer1() interrupt 3
{
    TH1 = TIMER1_H;
    TL1 = TIMER1_L;
    t1counter++;
    if (t1counter >= 20 / TL_SPEED_MULTIPLIER)
    {
        t1counter = 0;
        TL_HandleBlink();
    }
}

void main()
{
    uint8_t xdata keyNum = 0;
    INTR_Init();
    TL_Init();

    while (1)
    {
        // handle key input for maintenance mode toggle
        if (KEY_ScanStandalone_Busy(&keyNum))
        {
            if (keyNum == 1)
            {
                TL_SetMaintenanceMode(true);
            }
            else if (keyNum == 2)
            {
                TL_SetMaintenanceMode(false);
            }
        }
    }
}
