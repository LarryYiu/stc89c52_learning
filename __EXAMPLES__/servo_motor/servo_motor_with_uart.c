/*
 * @file: servo_motor_with_uart.c
 *
 * @brief:  THIS IS AN EXAMPLE OF USING UART TO CONTROL THE ROTATION DEGREE OF A
 * SERVO MOTOR. THE SERVO MOTOR IS CONTROLLED IN A WAY THAT THE USER CAN INPUT
 * THE ROTATION DEGREE IN THE SERIAL MONITOR, AND THE MOTOR WILL ROTATE TO A
 * TARGET DEGREE.
 *
 * @note: The servo is working under 50Hz(20ms) PWM signal, 0.5ms pulse width
 * for 0 degree, 2.5ms pulse width for 180 degree, and the pulse width is
 * linearly scaled between 0 and 180 degree. Step angle is assumed 9 degree for
 * this example.
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
#include "Servo.h"
#include "Uart.h"
#include "Utility.h"
#include "config.h"

uint8_t timer1counter = 0;  // Timer1 is used to determine whether the receiving
                            // of a string is complete. Timing 100ms and reset
                            // per character received.
uint8_t divisionOffset =
    0;  // This variable is used to determine the current position in the PWM
        // cycle for controlling the servo. It is incremented in Timer0
        // interrupt and reset after reaching SERVO_NUM_DIVISION_TOTAL.
bit receivedCompleteFlag = false;  // This flag is set to true when a complete
                                   // string is received via UART.
char
    receivedString[10];  // This buffer is used to store the characters received
                         // via UART until a complete string is formed.
uint8_t receivedLen = 0;  // This variable is used to keep track of the length
                          // of the received string.
uint8_t pulseWidth =
    SERVO_START_PW;  // This variable is used to store the pulse width
                     // corresponding to the desired servo angle. It is
                     // calculated based on the degree received via UART and
                     // updated in the main loop. SERVO_START_PW is the pulse
                     // width for 0 degree.

/**
 * @brief This timer is used to generate the PWM signal for controlling the
 * servo motor. The timer is configured to overflow at a frequency of 50Hz (20ms
 * period), and the interrupt service routine toggles the __SERVO_PIN__ based
 * on the current value of divisionOffset and pulseWidth to create the
 * appropriate pulse width for the desired servo angle.
 *
 * @note The servo is active HIGH, meaning that the pulse is generated when the
 * pin is HIGH.
 */
void Timer0() interrupt 1
{
    divisionOffset++;
    if (divisionOffset >= SERVO_NUM_DIVISION_TOTAL)
        divisionOffset = 0;
    if (divisionOffset < pulseWidth)
    {
        __SERVO_PIN__ = HIGH;
    }
    else
    {
        __SERVO_PIN__ = LOW;
    }
}

/**
 * @brief This timer is used to determine whether the receiving of a string is
 * complete. The timer is configured to overflow every 25ms, and the interrupt
 * service routine increments the timer1counter. If no new character is received
 * within 100ms (4 overflows), the receivedCompleteFlag is set to true.
 */
void Timer1() interrupt 3
{
    TH1 = MS_25_H;
    TL1 = MS_25_L;
    timer1counter++;
    if (timer1counter >= 4 && receivedLen > 0 && receivedCompleteFlag == false)
    {
        receivedCompleteFlag = true;
        timer1counter = 0;
    }
}

/**
 * @brief This interrupt service routine is triggered when a new character is
 * received via UART. It stores the received character in the receivedString
 * buffer and resets the timer1counter to determine the end of the string.
 */
void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;
        if (!receivedCompleteFlag)
        {
            receivedString[receivedLen++] = SBUF;
            timer1counter = 0;  // reset timer1counter every time a new
                                // character is received
        }
    }
}

void main()
{
    // INTR

    // DT
    uint8_t dat[6];
    uint8_t len;

    // Servo

    INTR_Init();
    UART_Init();

    len = DT_ProcessInt(pulseWidth, dat);
    while (1)
    {
        DT_DisplayMulti(dat, &len, -1, true);
        if (receivedCompleteFlag)
        {
            int16_t degree = atoi(receivedString);
            uint8_t i;
            if (degree > 180)
                degree = 180;
            else if (degree < 0)
                degree = 0;
            printf("Received degree: %d\n", degree);
            pulseWidth =
                SERVO_START_PW +
                (degree /
                 SERVO_DEGREE_DIVISION);  // Calculate the pulse width based on
                                          // the received degree and the defined
                                          // division for each degree.
            len = DT_ProcessInt(pulseWidth, dat);
            for (i = 0; i < receivedLen; i++)
            {
                receivedString[i] = 0;
            }
            receivedLen = 0;
            receivedCompleteFlag = false;
        }
    }
    return;
}