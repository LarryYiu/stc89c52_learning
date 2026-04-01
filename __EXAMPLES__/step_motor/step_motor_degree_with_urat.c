/*
 * @file: step_motor_degree_with_urat.c
 *
 * @brief:  THIS IS AN EXAMPLE OF USING UART TO CONTROL THE ROTATION DEGREE OF A
 * STEP MOTOR. THE STEP MOTOR IS CONTROLLED IN A WAY THAT THE USER CAN INPUT THE
 * ROTATION DEGREE IN THE SERIAL MONITOR, AND THE MOTOR WILL ROTATE TO A
 * TARGET DEGREE WITH A FIXED SPEED. THE USER CAN ALSO INPUT NEGATIVE DEGREE TO
 * CONTROL THE ROTATION DIRECTION.
 *
 * @warning:  THIS IS AN EXAMPLE CODE FOR DEMONSTRATION PURPOSE ONLY. IT IS NOT
 * INTENDED TO BE USED IN PRODUCTION ENVIRONMENT AS IT MAY CONTAIN BUGS AND
 * SECURITY VULNERABILITIES. USE IT AT YOUR OWN RISK.
 */

#include <STC89C5xRC.H>
#include <stdio.h>
#include <stdlib.h>

#include "DigitalTube.h"
#include "Interrupt.h"
#include "StepMotor.h"
#include "Uart.h"
#include "Utility.h"
#include "config.h"

uint16_t timer1counter = 0;
uint16_t timer0counter = 0;
bit receivedCompleteFlag = false;
char receivedSting[10];
uint8_t receivedLen = 0;

/**
 * @brief This timer is used to determine whether the receiving of a string is
 * complete. The timer will be reset every time a new character is received, and
 * if the timer reaches 100ms without receiving a new character, it will be
 * considered that the string has been completely received.
 */
void Timer0() interrupt 1
{
    TH0 = MS_25_H;
    TL0 = MS_25_L;
    timer0counter++;
    if (timer0counter >= 4 && receivedLen > 0 && receivedCompleteFlag == false)
    {
        receivedCompleteFlag = true;
        timer0counter = 0;
    }
}

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

/**
 * @brief This interrupt is triggered when a new character is received via UART.
 * It stores the received character in a buffer and resets the timer0counter to
 * determine the end of the string.
 */
void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;
        if (!receivedCompleteFlag)
        {
            receivedSting[receivedLen++] = SBUF;
            timer0counter = 0;  // reset timer0counter every time a new
                                // character is received
        }
    }
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
    int16_t stepsLeft = 0;

    INTR_Init();
    UART_Init();
    StepMotor_Init(&motor);
    motor.speedScaler =
        10;  // Set the speed scaler to control the rotation speed of the motor,
             // the SMALLER the value, the FASTER the motor will rotate.

    len = DT_ProcessInt(stepsLeft, dat);
    while (1)
    {
        DT_DisplayMulti(dat, &len, -1, true);
        if (receivedCompleteFlag)
        {
            int16_t degree;
            uint8_t i;
            if (receivedSting[0] == '-')
            {
                motor.ccw = true;
                receivedSting[0] = ' ';
            }
            else
            {
                motor.ccw = false;
            }
            degree = atoi(receivedSting);
            stepsLeft = (degree % 360) * STEP_MOTOR_DEGREE_PER_STEP;
            printf("direction: %s, degree: %d, stepsLeft: %d\n",
                   motor.ccw ? "CCW" : "CW", degree, stepsLeft);
            for (i = 0; i < receivedLen; i++)
            {
                receivedSting[i] = 0;
            }
            receivedLen = 0;
            receivedCompleteFlag = false;
        }

        if (stepsLeft == 0)
        {
            P1 = 0xff;
            timer1counter = 0;
            tik = 0;
            continue;
        }
        if (timer1counter - tik >= motor.speedScaler)
        {
            tik = timer1counter;
            StepMotor_HandleRotation(&motor);
            stepsLeft--;
            len = DT_ProcessInt(stepsLeft, dat);
        }
    }
    return;
}