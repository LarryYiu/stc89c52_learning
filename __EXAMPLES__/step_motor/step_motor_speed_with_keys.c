/*
 * @file: step_motor_speed_with_keys.c
 *
 * @brief: This example demonstrates how to control the speed of a step motor
 * using keys. The speed can be increased, decreased, reset, and the rotation
 * direction can be toggled using different keys. The current speed is displayed
 * on a digital tube.
 *
 * @warning:  THIS IS AN EXAMPLE CODE FOR DEMONSTRATION PURPOSE ONLY. IT IS NOT
 * INTENDED TO BE USED IN PRODUCTION ENVIRONMENT AS IT MAY CONTAIN BUGS AND
 * SECURITY VULNERABILITIES. USE IT AT YOUR OWN RISK.
 */

#include <STC89C5xRC.H>
#include <stdio.h>

#include "DigitalTube.h"
#include "Interrupt.h"
#include "Key.h"
#include "StepMotor.h"
#include "Utility.h"
#include "config.h"

uint8_t pulseWidth = 0;
uint8_t intervalOffset = 0;
uint16_t timer1counter = 0;
// uint8_t code stepMotorTable[4] = {0xf1, 0xf2, 0xf4, 0xf8};

void Timer0() interrupt 1 {}

/**
 * @brief This timer is used to control the speed of the step motor. The motor
 * in example works from 500Hz to 900Hz.
 */
void Timer1() interrupt 3
{
    TH1 = MS_1_H;
    TL1 = MS_1_L;
    timer1counter++;
}

void main()
{
    // INTR
    uint32_t tik = 0;

    // KEY
    uint8_t keyNum = 0;

    // DT
    uint8_t dat[6];
    uint8_t len;

    // StepMotor
    StepMotor motor;

    INTR_Init();
    StepMotor_Init(&motor);

    len = DT_ProcessInt(motor.speedScaler, dat);
    while (1)
    {
        if (timer1counter - tik >= motor.speedScaler)
        {
            tik = timer1counter;
            StepMotor_HandleRotation(&motor);
        }
        DT_DisplayMulti(dat, &len, -1, true);
        if (KEY_ScanStandalone_Busy(&keyNum))
        {
            if (keyNum == 1)
            {
                motor.speedScaler++;
            }
            else if (keyNum == 2)
            {
                motor.speedScaler--;
            }
            else if (keyNum == 3)
            {
                motor.speedScaler = 10;
            }
            else if (keyNum == 4)
            {
                motor.ccw = !motor.ccw;
            }
            len = DT_ProcessInt(motor.speedScaler, dat);
        }
    }
    return;
}