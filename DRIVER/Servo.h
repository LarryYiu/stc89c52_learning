#ifndef __SERVO_H__
#define __SERVO_H__
#include <STC89C5xRC.H>

#include "Utility.h"
#include "config.h"

#ifndef __SERVO_PIN__
#define __SERVO_PIN__ P17
#endif  // __SERVO_PIN__

#ifndef SERVO_HIGH_ACTIVE
#define SERVO_HIGH_ACTIVE true
#endif  // SERVO_HIGH_ACTIVE

#ifndef SERVO_NUM_DIVISION_TOTAL
#ifndef SERVO_DELTA_TIME
#define SERVO_DELTA_TIME 20000  // us
#endif                          // SERVO_DELTA_TIME

#ifndef SERVO_END_TIME
#define SERVO_END_TIME 2500  // us
#endif                       // SERVO_END_TIME

#ifndef SERVO_START_TIME
#define SERVO_START_TIME 500  // us
#endif                        // SERVO_START_TIME

#ifndef SERVO_DEGREE_DIVISION
#define SERVO_DEGREE_DIVISION 9  // degree per division
#endif                           // SERVO_DEGREE_DIVISION

#ifndef SERVO_NUM_DIVISION_PW
#define SERVO_NUM_DIVISION_PW \
    ((unsigned int)((180UL) / (SERVO_DEGREE_DIVISION)))
#endif  // SERVO_NUM_DIVISION_PW

#define SERVO_NUM_DIVISION_TOTAL              \
    ((unsigned long)(SERVO_NUM_DIVISION_PW) * \
     (unsigned long)(SERVO_DELTA_TIME) /      \
     ((unsigned long)(SERVO_END_TIME) - (unsigned long)(SERVO_START_TIME)))

#ifndef SERVO_START_PW
#define SERVO_START_PW                   \
    ((unsigned long)(SERVO_START_TIME) * \
     (unsigned long)(SERVO_NUM_DIVISION_PW) / (unsigned long)(SERVO_END_TIME))
#endif  // SERVO_START_PW

#ifndef SERVO_END_PW
#define SERVO_END_PW \
    ((unsigned long)(SERVO_START_PW) + (unsigned long)(SERVO_NUM_DIVISION_PW))
#endif  // SERVO_END_PW

#endif  // SERVO_NUM_DIVISION_TOTAL

/**
 * @brief This function is the interrupt service routine for Timer0, which is
 * used to generate the PWM signal for controlling the servo motor. It updates
 * the divisionOffset and toggles the __SERVO_PIN__ based on the current value
 * of divisionOffset and pulseWidth to create the appropriate pulse width for
 * the desired servo angle.
 *
 * @note This function should be called in the Timer0 interrupt service
 * routine to ensure proper timing and functionality of the servo control.
 *
 * @warning this function may cause an unexpected behavior, it is recommended to
 * use the Timer0 interrupt service routine in
 * __EXAMPLES__/servo_motor/servo_motor_with_uart.c as a reference for
 * implementing the servo control logic, and to ensure that the timing and pulse
 * width calculations are consistent with the defined constants and
 * configurations in this header file.
 *
 * @param divisionOffset A pointer to the current division offset, which is
 * updated in each interrupt to determine the timing of the PWM signal.
 *
 * @param pulseWidth A pointer to the current pulse width, which is used to
 * generate the appropriate PWM signal for the servo motor.
 */
void Servo_HandleInterrupt(uint8_t* divisionOffset, const uint8_t* pulseWidth);

/**
 * @brief This function updates the pulse width based on the desired servo angle
 * provided as an input parameter.
 *
 * @param degree A pointer to the desired servo angle in degrees.
 * @param pulseWidth A pointer to the pulse width variable, which will be
 * updated based on the desired servo angle.
 */
void Servo_UpdatePulseWidth(int16_t* degree, uint8_t* pulseWidth);

#endif  // __SERVO_H__