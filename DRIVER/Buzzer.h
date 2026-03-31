#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "STC89C5xRC.H"
#include "Utility.h"

#define __PIN_BUZZER__ P23

/**
 * @brief Initialize the buzzer by setting the control pin to the appropriate
 * level.
 */
void BUZZER_Init();

/**
 * @brief Turn on the buzzer by setting the control pin to the appropriate
 * level.
 */
void BUZZER_On();

/**
 * @brief Turn off the buzzer by setting the control pin to the appropriate
 * level.
 */
void BUZZER_Off();

/**
 * @brief Make the buzzer beep for a specified duration in milliseconds.
 *
 * @warning This function uses busy-waiting (blocking delay), which may not be
 * suitable for all applications.
 *
 * @param duration_ms The duration for which the buzzer should beep, in
 * milliseconds.
 */
void BUZZER_Beep(uint16_t duration_ms);

#endif  // __BUZZER_H__