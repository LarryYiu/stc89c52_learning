/**
 * @file StepMotor.h
 * @author *
 * @brief THIS IS A 5 CABLES UNIPOLAR STEP MOTOR DRIVER. THE MOTOR IS
 * CONNECTED TO PORT 1 BY DEFAULT, AND THE CONTROL PINS ARE CONNECTED TO P1.0,
 * P1.1, P1.2, P1.3 RESPECTIVELY.
 * @version 0.1
 */

#ifndef __STEPMOTOR_H__
#define __STEPMOTOR_H__

#include <STC89C5xRC.H>

#include "Utility.h"
#include "config.h"

#ifndef __STEP_MOTOR_PORT__
#define __STEP_MOTOR_PORT__ P1
#endif  // __STEP_MOTOR_PORT__

#ifndef STEP_MOTOR_DEFAULT_SPEED_SCALER
#define STEP_MOTOR_DEFAULT_SPEED_SCALER 10
#endif  // STEP_MOTOR_DEFAULT_SPEED_SCALER

#ifndef STEP_MONTOR_STEP_ANGLE
#define STEP_MONTOR_STEP_ANGLE 5.625
#endif  // STEP_MONTOR_STEP_ANGLE

#ifndef STEP_MOTOR_STEP_DEC_RATIO
#define STEP_MOTOR_STEP_DEC_RATIO 64
#endif  // STEP_MOTOR_STEP_DEC_RATIO

#ifndef STEP_MOTOR_FOUR_PHASE_FOUR_BEAT
#define STEP_MOTOR_FOUR_PHASE_FOUR_BEAT true
#endif  // STEP_MOTOR_FOUR_PHASE_FOUR_BEAT

#if (STEP_MOTOR_FOUR_PHASE_FOUR_BEAT)
#define STEP_MOTOR_DEGREE_PER_STEP \
    (STEP_MOTOR_STEP_DEC_RATIO / STEP_MONTOR_STEP_ANGLE / 2)
#else
#define STEP_MOTOR_DEGREE_PER_STEP \
    (STEP_MOTOR_STEP_DEC_RATIO / STEP_MONTOR_STEP_ANGLE)
#endif  // STEP_MOTOR_FOUR_PHASE_FOUR_BEAT

typedef struct
{
    uint8_t speedScaler;
    uint8_t stepIndex;
    bool ccw;
} StepMotor;

void StepMotor_Init(StepMotor* motor);

void StepMotor_StepCW(StepMotor* motor);
void StepMotor_StepCCW(StepMotor* motor);

void StepMotor_HandleRotation(StepMotor* motor);

#endif  // __STEPMOTOR_H__