#include "DC_Motor.h"

void DC_MOTOR_Init()
{
#if (DC_MOTOR_ACTIVE_HIGH)
    __DC_MOTOR_PIN__ = LOW;
#else
    __DC_MOTOR_PIN__ = HIGH;
#endif  // DC_MOTOR_ACTIVE_HIGH
}

void DC_MOTOR_On()
{
#if (DC_MOTOR_ACTIVE_HIGH)
    __DC_MOTOR_PIN__ = HIGH;
#else
    __DC_MOTOR_PIN__ = LOW;
#endif  // DC_MOTOR_ACTIVE_HIGH
}

void DC_MOTOR_Off()
{
#if (DC_MOTOR_ACTIVE_HIGH)
    __DC_MOTOR_PIN__ = LOW;
#else
    __DC_MOTOR_PIN__ = HIGH;
#endif  // DC_MOTOR_ACTIVE_HIGH
}