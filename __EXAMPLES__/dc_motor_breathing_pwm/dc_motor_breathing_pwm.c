#include <STC89C5xRC.H>
#include <stdio.h>

#include "DC_Motor.h"
#include "DigitalTube.h"
#include "Interrupt.h"
#include "Key.h"
#include "Utility.h"
#include "config.h"

/*
 * This code is an example of controlling a DC motor using Pulse Width
 * Modulation (PWM) with an STC89C52 microcontroller. The goal is to add a
 * breath effect to the motor speed, which means that the speed will gradually
 * increase and decrease in a smooth manner, creating a breathing-like effect.
 *
 * Keys are used to adjust the breathing interval.
 */

uint8_t pulseWidth =
    5;  // variable represents the pulse width of the PWM signal, which is used
        // to control the speed of the DC motor. The value can be adjusted to
        // increase or decrease the duty cycle of the PWM signal, which in turn
        // affects the speed of the motor.
uint8_t intervalOffset =
    0;  // variable repsents the time offset of the pulse width, which is used
        // to adjust the duty cycle of the PWM signal
uint8_t timer1counter =
    0;  // variable represents the interrupt counter for Timer1, which is used
        // to determine when to update the pulse width of the PWM signal.
uint8_t intervalScaler = 1;  // variable represents time interval in a scale of
                             // 500ms between each division of pulse width.
bool isRisingPW =
    true;  // variable represents whether the pulse width is currently rising or
           // falling, which is used to determine whether to increase or
           // decrease the pulse width in the Timer1 interrupt service routine.

/*
 * Divide a cycle of PWM into 10 intervals, and turn on the motor when the
 * current intervalOffset is less than the pulse width threshold (pulseWidth).
 */
void Timer0() interrupt 1
{
    intervalOffset++;
    if (intervalOffset >= 10)
        intervalOffset = 0;
    if (intervalOffset < pulseWidth)
    {
        DC_MOTOR_On();
    }
    else
    {
        DC_MOTOR_Off();
    }
}

/*
 * This routine is used to update the pulse width along with the timer
 * interrupt, which occurs at regular intervals. The intervalScaler variable is
 * used to control the speed of the breathing effect, and the isRisingPW
 * variable is used to determine whether the pulse width is currently rising or
 * falling.
 *
 * Each divison of pulse width starts at 25ms, as the intervalScaler is 1, with
 * all 20 divisons one cycle last for 500ms. Adding one to the intervalScaler
 * will add 500ms to the cycle length.
 *
 */
void timer1() interrupt 3
{
    TH1 = MS_25_H;
    TL1 = MS_25_L;
    if (++timer1counter >= intervalScaler)
    {
        timer1counter = 0;
        if (isRisingPW)
        {
            if (pulseWidth >= 10)
            {
                isRisingPW = false;
                pulseWidth--;
            }
            else
            {
                pulseWidth++;
            }
        }
        else
        {
            if (pulseWidth <= 0)
            {
                isRisingPW = true;
                pulseWidth++;
            }
            else
            {
                pulseWidth--;
            }
        }
    }
}

void main()
{
    uint8_t keyNum = 0;
    uint8_t len;
    uint8_t dat[6];
    INTR_Init();
    DC_MOTOR_Init();
    len = DT_ProcessInt(intervalScaler * 500, dat);
    while (1)
    {
        DT_DisplayMulti(dat, &len, -1, true);
        if (KEY_ScanStandalone_Busy(&keyNum))
        {
            if (keyNum == 1)
            {
                intervalScaler = intervalScaler >= 10 ? 1 : intervalScaler + 1;
            }
            else if (keyNum == 2)
            {
                intervalScaler = intervalScaler <= 1 ? 10 : intervalScaler - 1;
            }
            len = DT_ProcessInt(intervalScaler * 500, dat);
        }
    }
    return;
}