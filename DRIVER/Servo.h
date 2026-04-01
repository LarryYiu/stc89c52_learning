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

void Servo_HandleInterrupt(uint8_t* divisionOffset, const uint8_t* pulseWidth);

void Servo_UpdatePulseWidth(int16_t* degree, uint8_t* pulseWidth);

#endif  // __SERVO_H__