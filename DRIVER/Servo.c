#include "Servo.h"

// ERROR HERE
void Servo_HandleInterrupt(uint8_t* divisionOffset, const uint8_t* pulseWidth)
{
    (*divisionOffset)++;
    if (*divisionOffset >= SERVO_NUM_DIVISION_TOTAL)
        *divisionOffset = 0;
    if (*divisionOffset < *pulseWidth)
    {
#if (SERVO_HIGH_ACTIVE)
        __SERVO_PIN__ = HIGH;
#else
        __SERVO_PIN__ = LOW;
#endif  // SERVO_HIGH_ACTIVE
    }
    else
    {
#if (SERVO_HIGH_ACTIVE)
        __SERVO_PIN__ = LOW;
#else
        __SERVO_PIN__ = HIGH;
#endif  // SERVO_HIGH_ACTIVE
    }
}

void Servo_UpdatePulseWidth(int16_t* degree, uint8_t* pulseWidth)
{
    if (*degree > 180)
        *degree = 180;
    else if (*degree < 0)
        *degree = 0;

    *pulseWidth = SERVO_START_PW + (*degree / SERVO_DEGREE_DIVISION);
}