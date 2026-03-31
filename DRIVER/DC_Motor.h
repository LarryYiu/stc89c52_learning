#ifndef __DC_MOTOR_H__
#define __DC_MOTOR_H__

#include <STC89C5xRC.H>

#include "Utility.h"
#include "config.h"

#define __DC_MOTOR_PIN__ P17

#ifndef DC_MOTOR_ACTIVE_HIGH
#define DC_MOTOR_ACTIVE_HIGH true
#endif  // DC_MOTOR_ACTIVE_HIGH

/**
 * @brief Initialize the DC motor.
 */
void DC_MOTOR_Init();

/**
 * @brief Turn on the DC motor by setting the control pin to the appropriate
 * level.
 */
void DC_MOTOR_On();

/**
 * @brief Turn off the DC motor by setting the control pin to the appropriate
 * level.
 */
void DC_MOTOR_Off();

#endif  // __DC_MOTOR_H__