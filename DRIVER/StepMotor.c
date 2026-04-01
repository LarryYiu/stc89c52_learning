#include "StepMotor.h"

uint8_t code stepMotorTable[4] = {0xf1, 0xf2, 0xf4, 0xf8};

void StepMotor_Init(StepMotor* motor)
{
    // __STEPMOTOR_PIN0__ = 1;
    // __STEPMOTOR_PIN1__ = 1;
    // __STEPMOTOR_PIN2__ = 1;
    // __STEPMOTOR_PIN3__ = 1;
    motor->speedScaler = STEP_MOTOR_DEFAULT_SPEED_SCALER;
    motor->stepIndex = 0;
    motor->ccw = false;
}

void StepMotor_StepCW(StepMotor* motor)
{
    motor->stepIndex = (motor->stepIndex + 1) % 4;
    __STEP_MOTOR_PORT__ = stepMotorTable[motor->stepIndex];
}

void StepMotor_StepCCW(StepMotor* motor)
{
    motor->stepIndex = (motor->stepIndex + 3) % 4;
    __STEP_MOTOR_PORT__ = stepMotorTable[motor->stepIndex];
}

void StepMotor_HandleRotation(StepMotor* motor)
{
    motor->ccw ? StepMotor_StepCCW(motor) : StepMotor_StepCW(motor);
}